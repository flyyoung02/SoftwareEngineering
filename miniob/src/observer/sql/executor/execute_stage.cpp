/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Meiyi & Longda on 2021/4/13.
//

#include <string>
#include <sstream>

#include "execute_stage.h"

#include "common/io/io.h"
#include "common/log/log.h"
#include "common/lang/defer.h"
#include "common/seda/timer_stage.h"
#include "common/lang/string.h"
#include "session/session.h"
#include "event/storage_event.h"
#include "event/sql_event.h"
#include "event/session_event.h"
#include "sql/expr/tuple.h"
#include "sql/operator/table_scan_physical_operator.h"
#include "sql/operator/index_scan_physical_operator.h"
#include "sql/operator/predicate_physical_operator.h"
#include "sql/operator/delete_physical_operator.h"
#include "sql/operator/project_physical_operator.h"
#include "sql/operator/string_list_physical_operator.h"
#include "sql/stmt/stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/update_stmt.h"
#include "sql/stmt/delete_stmt.h"
#include "sql/stmt/insert_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/common/table.h"
#include "storage/common/field.h"
#include "storage/index/index.h"
#include "storage/default/default_handler.h"
#include "storage/common/condition_filter.h"
#include "storage/trx/trx.h"
#include "storage/clog/clog.h"

using namespace common;

//! Constructor
ExecuteStage::ExecuteStage(const char *tag) : Stage(tag)
{}

//! Destructor
ExecuteStage::~ExecuteStage()
{}

//! Parse properties, instantiate a stage object
Stage *ExecuteStage::make_stage(const std::string &tag)
{
  ExecuteStage *stage = new (std::nothrow) ExecuteStage(tag.c_str());
  if (stage == nullptr) {
    LOG_ERROR("new ExecuteStage failed");
    return nullptr;
  }
  stage->set_properties();
  return stage;
}

//! Set properties for this object set in stage specific properties
bool ExecuteStage::set_properties()
{
  //  std::string stageNameStr(stageName);
  //  std::map<std::string, std::string> section = theGlobalProperties()->get(
  //    stageNameStr);
  //
  //  std::map<std::string, std::string>::iterator it;
  //
  //  std::string key;

  return true;
}

//! Initialize stage params and validate outputs
bool ExecuteStage::initialize()
{
  LOG_TRACE("Enter");

  std::list<Stage *>::iterator stgp = next_stage_list_.begin();
  default_storage_stage_ = *(stgp++);
  mem_storage_stage_ = *(stgp++);

  LOG_TRACE("Exit");
  return true;
}

//! Cleanup after disconnection
void ExecuteStage::cleanup()
{
  LOG_TRACE("Enter");

  LOG_TRACE("Exit");
}

void ExecuteStage::handle_event(StageEvent *event)
{
  LOG_TRACE("Enter");

  handle_request(event);

  LOG_TRACE("Exit");
  return;
}

void ExecuteStage::callback_event(StageEvent *event, CallbackContext *context)
{
  LOG_TRACE("Enter");

  // here finish read all data from disk or network, but do nothing here.

  LOG_TRACE("Exit");
  return;
}

RC ExecuteStage::handle_request(common::StageEvent *event)
{
  SQLStageEvent *sql_event = static_cast<SQLStageEvent *>(event);

  const std::unique_ptr<PhysicalOperator> &physical_operator = sql_event->physical_operator();
  if (physical_operator != nullptr) {
    return handle_request_with_physical_operator(sql_event);
  }

  SessionEvent *session_event = sql_event->session_event();
  Session *session = session_event->session();

  Command *sql = sql_event->command().get();
  Stmt *stmt = sql_event->stmt();
  if (stmt != nullptr) {
    switch (stmt->type()) {
      case StmtType::INSERT: {
        do_insert(sql_event);
      } break;
      case StmtType::UPDATE: {
        // do_update((UpdateStmt *)stmt, session_event);
      } break;
      case StmtType::DELETE: {
        do_delete(sql_event);
      } break;
      default: {
        LOG_WARN("should not happen. please implement this type:%d", stmt->type());
      } break;
    }
  } else {
    switch (sql->flag) {
      case SCF_HELP: {
        do_help(sql_event);
      } break;
      case SCF_CREATE_TABLE: {
        do_create_table(sql_event);
      } break;
      case SCF_CREATE_INDEX: {
        do_create_index(sql_event);
      } break;
      case SCF_SHOW_TABLES: {
        do_show_tables(sql_event);
      } break;
      case SCF_DESC_TABLE: {
        do_desc_table(sql_event);
      } break;

      case SCF_DROP_TABLE:
      case SCF_DROP_INDEX:
      case SCF_LOAD_DATA: {
        default_storage_stage_->handle_event(event);
      } break;
      case SCF_SYNC: {
        /*
        RC rc = DefaultHandler::get_default().sync();
        session_event->set_response(strrc(rc));
        */
      } break;
      case SCF_BEGIN: {
        do_begin(sql_event);
        /*
        session_event->set_response("SUCCESS\n");
        */
      } break;
      case SCF_COMMIT: {
        do_commit(sql_event);
        /*
        Trx *trx = session->current_trx();
        RC rc = trx->commit();
        session->set_trx_multi_operation_mode(false);
        session_event->set_response(strrc(rc));
        */
      } break;
      case SCF_CLOG_SYNC: {
        do_clog_sync(sql_event);
      }
      case SCF_ROLLBACK: {
        Trx *trx = session_event->session()->current_trx();

        RC rc = trx->rollback();

        session->set_trx_multi_operation_mode(false);

        SqlResult *sql_result = new SqlResult;
        sql_result->set_return_code(rc);

        session_event->set_sql_result(sql_result);
      } break;
      case SCF_EXIT: {
        // do nothing
        SqlResult *sql_result = new SqlResult;
        sql_result->set_return_code(RC::SUCCESS);

        session_event->set_sql_result(sql_result);
      } break;
      default: {

        LOG_ERROR("Unsupported command=%d\n", sql->flag);

        SqlResult *sql_result = new SqlResult;

        sql_result->set_return_code(RC::UNIMPLENMENT);
        sql_result->set_state_string("Unsupported command");

        session_event->set_sql_result(sql_result);
      }
    }
  }
  return RC::SUCCESS;
}

RC ExecuteStage::handle_request_with_physical_operator(SQLStageEvent *sql_event)
{
  RC rc = RC::SUCCESS;

  Stmt *stmt = sql_event->stmt();
  ASSERT(stmt != nullptr, "SQL Statement shouldn't be empty!");

  std::unique_ptr<PhysicalOperator> &physical_operator = sql_event->physical_operator();
  ASSERT(physical_operator != nullptr, "physical operator should not be null");

  TupleSchema schema;
  switch (stmt->type()) {
    case StmtType::SELECT: {
      SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);
      bool with_table_name = select_stmt->tables().size() > 1;

      for (const Field &field : select_stmt->query_fields()) {
        if (with_table_name) {
          schema.append_cell(field.table_name(), field.field_name());
        } else {
          schema.append_cell(field.field_name());
        }
      }
    } break;

    case StmtType::EXPLAIN: {
      schema.append_cell("Query Plan");
    } break;
    default: {
      // 只有select返回结果
    } break;
  }

  SqlResult *sql_result = new SqlResult;
  sql_result->set_tuple_schema(schema);
  sql_result->set_operator(std::move(physical_operator));

  sql_event->session_event()->set_sql_result(sql_result);
  return rc;
}

void end_trx_if_need(Session *session, Trx *trx, bool all_right)
{
  if (!session->is_trx_multi_operation_mode()) {
    if (all_right) {
      trx->commit();
    } else {
      trx->rollback();
    }
  }
}

void tuple_to_string(std::ostream &os, const Tuple &tuple)
{
  RC rc = RC::SUCCESS;
  bool first_field = true;

  TupleCell cell;
  for (int i = 0; i < tuple.cell_num(); i++) {
    rc = tuple.cell_at(i, cell);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to fetch field of cell. index=%d, rc=%s", i, strrc(rc));
      break;
    }

    if (!first_field) {
      os << " | ";
    } else {
      first_field = false;
    }
    cell.to_string(os);
  }
}

#if 0
IndexScanOperator *try_to_create_index_scan_operator(FilterStmt *filter_stmt)
{
  const std::vector<FilterUnit *> &filter_units = filter_stmt->filter_units();
  if (filter_units.empty() ) {
    return nullptr;
  }

  // 在所有过滤条件中，找到字段与值做比较的条件，然后判断字段是否可以使用索引
  // 如果是多列索引，这里的处理需要更复杂。
  // 这里的查找规则是比较简单的，就是尽量找到使用相等比较的索引
  // 如果没有就找范围比较的，但是直接排除不等比较的索引查询. (你知道为什么?)
  const FilterUnit *better_filter = nullptr;
  for (const FilterUnit * filter_unit : filter_units) {
    if (filter_unit->comp() == NOT_EQUAL) {
      continue;
    }

    Expression *left = filter_unit->left();
    Expression *right = filter_unit->right();
    if (left->type() == ExprType::FIELD && right->type() == ExprType::VALUE) {
    } else if (left->type() == ExprType::VALUE && right->type() == ExprType::FIELD) {
      std::swap(left, right);
    } else {
      continue;
    }

    FieldExpr &left_field_expr = *(FieldExpr *)left;
    const Field &field = left_field_expr.field();
    const Table *table = field.table();
    Index *index = table->find_index_by_field(field.field_name());
    if (index != nullptr) {
      if (better_filter == nullptr) {
        better_filter = filter_unit;
      } else if (filter_unit->comp() == EQUAL_TO) {
        better_filter = filter_unit;
    	break;
      }
    }
  }

  if (better_filter == nullptr) {
    return nullptr;
  }

  Expression *left = better_filter->left();
  Expression *right = better_filter->right();
  CompOp comp = better_filter->comp();
  if (left->type() == ExprType::VALUE && right->type() == ExprType::FIELD) {
    std::swap(left, right);
    switch (comp) {
    case EQUAL_TO:    { comp = EQUAL_TO; }    break;
    case LESS_EQUAL:  { comp = GREAT_THAN; }  break;
    case NOT_EQUAL:   { comp = NOT_EQUAL; }   break;
    case LESS_THAN:   { comp = GREAT_EQUAL; } break;
    case GREAT_EQUAL: { comp = LESS_THAN; }   break;
    case GREAT_THAN:  { comp = LESS_EQUAL; }  break;
    default: {
    	LOG_WARN("should not happen");
    }
    }
  }

  FieldExpr &left_field_expr = *(FieldExpr *)left;
  const Field &field = left_field_expr.field();
  const Table *table = field.table();
  Index *index = table->find_index_by_field(field.field_name());
  assert(index != nullptr);

  ValueExpr &right_value_expr = *(ValueExpr *)right;
  TupleCell value;
  right_value_expr.get_tuple_cell(value);

  const TupleCell *left_cell = nullptr;
  const TupleCell *right_cell = nullptr;
  bool left_inclusive = false;
  bool right_inclusive = false;

  switch (comp) {
  case EQUAL_TO: {
    left_cell = &value;
    right_cell = &value;
    left_inclusive = true;
    right_inclusive = true;
  } break;

  case LESS_EQUAL: {
    left_cell = nullptr;
    left_inclusive = false;
    right_cell = &value;
    right_inclusive = true;
  } break;

  case LESS_THAN: {
    left_cell = nullptr;
    left_inclusive = false;
    right_cell = &value;
    right_inclusive = false;
  } break;

  case GREAT_EQUAL: {
    left_cell = &value;
    left_inclusive = true;
    right_cell = nullptr;
    right_inclusive = false;
  } break;

  case GREAT_THAN: {
    left_cell = &value;
    left_inclusive = false;
    right_cell = nullptr;
    right_inclusive = false;
  } break;

  default: {
    LOG_WARN("should not happen. comp=%d", comp);
  } break;
  }

  IndexScanOperator *oper = new IndexScanOperator(table, index,
       left_cell, left_inclusive, right_cell, right_inclusive);

  LOG_INFO("use index for scan: %s in table %s", index->index_meta().name(), table->name());
  return oper;
}

#endif

RC ExecuteStage::do_select(SQLStageEvent *sql_event)
{
#if 0
  RC rc = RC::SUCCESS;

  SessionEvent *session_event = sql_event->session_event();

  SelectStmt *select_stmt = (SelectStmt *)(sql_event->stmt());
  if (select_stmt->tables().size() != 1) {
    LOG_WARN("select more than 1 tables is not supported");
    rc = RC::UNIMPLENMENT;
    return rc;
  }

  Operator *scan_oper = nullptr; // try_to_create_index_scan_operator(select_stmt->filter_stmt());
  if (nullptr == scan_oper) {
    scan_oper = new TableScanOperator(select_stmt->tables()[0]);
  }

  PredicateOperator *pred_oper = new PredicateOperator(select_stmt->filter_stmt());
  pred_oper->add_child(scan_oper);

  ProjectOperator *project_oper = new ProjectOperator;
  project_oper->add_child(pred_oper);

  TupleSchema schema;
  for (const Field &field : select_stmt->query_fields()) {
    project_oper->add_projection(field.table(), field.meta());
    schema.append_cell(field.field_name());
  }

  SqlResult *sql_result = new SqlResult;
  sql_result->set_tuple_schema(schema);
  sql_result->set_operator(project_oper);

  /*
  rc = project_oper.open();
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open operator");
    return rc;
  }

  std::stringstream ss;
  print_tuple_header(ss, project_oper);
  while ((rc = project_oper.next()) == RC::SUCCESS) {
    // get current record
    // write to response
    Tuple * tuple = project_oper.current_tuple();
    if (nullptr == tuple) {
      rc = RC::INTERNAL;
      LOG_WARN("failed to get current record. rc=%s", strrc(rc));
      break;
    }

    tuple_to_string(ss, *tuple);
    ss << std::endl;
  }

  if (rc != RC::RECORD_EOF) {
    LOG_WARN("something wrong while iterate operator. rc=%s", strrc(rc));
    project_oper.close();
  } else {
    rc = project_oper.close();
  }
  session_event->set_response(ss.str());
  */
  session_event->set_sql_result(sql_result);
#endif
  return RC::SUCCESS;
}

RC ExecuteStage::do_help(SQLStageEvent *sql_event)
{
  SessionEvent *session_event = sql_event->session_event();

  const char *strings[] = {"show tables;",
      "desc `table name`;",
      "create table `table name` (`column name` `column type`, ...);",
      "create index `index name` on `table` (`column`);",
      "insert into `table` values(`value1`,`value2`);",
      "update `table` set column=value [where `column`=`value`];",
      "delete from `table` [where `column`=`value`];",
      "select [ * | `columns` ] from `table`;"};

  auto oper = new StringListPhysicalOperator();
  for (size_t i = 0; i < sizeof(strings) / sizeof(strings[0]); i++) {
    oper->append(strings[i]);
  }

  SqlResult *sql_result = new SqlResult;

  TupleSchema schema;
  schema.append_cell("Commands");

  sql_result->set_tuple_schema(schema);
  sql_result->set_operator(std::unique_ptr<PhysicalOperator>(oper));

  session_event->set_sql_result(sql_result);
  return RC::SUCCESS;
}

RC ExecuteStage::do_create_table(SQLStageEvent *sql_event)
{
  SessionEvent *session_event = sql_event->session_event();
  Db *db = session_event->session()->get_current_db();

  const CreateTable &create_table = sql_event->command()->create_table;
  const int attribute_count = static_cast<int>(create_table.attr_infos.size());

  RC rc = db->create_table(create_table.relation_name.c_str(), attribute_count, create_table.attr_infos.data());

  SqlResult *sql_result = new SqlResult;
  sql_result->set_return_code(rc);

  sql_event->session_event()->set_sql_result(sql_result);
  return rc;
}
RC ExecuteStage::do_create_index(SQLStageEvent *sql_event)
{
  SqlResult *sql_result = new SqlResult;
  SessionEvent *session_event = sql_event->session_event();
  session_event->set_sql_result(sql_result);

  Db *db = session_event->session()->get_current_db();

  const CreateIndex &create_index = sql_event->command()->create_index;
  Table *table = db->find_table(create_index.relation_name.c_str());
  if (nullptr == table) {
    sql_result->set_return_code(RC::SCHEMA_TABLE_NOT_EXIST);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  RC rc = table->create_index(nullptr, create_index.index_name.c_str(), create_index.attribute_name.c_str());

  sql_result->set_return_code(rc);
  return rc;
}

RC ExecuteStage::do_show_tables(SQLStageEvent *sql_event)
{
  SqlResult *sql_result = new SqlResult;
  SessionEvent *session_event = sql_event->session_event();
  session_event->set_sql_result(sql_result);

  Db *db = session_event->session()->get_current_db();

  std::vector<std::string> all_tables;
  db->all_tables(all_tables);

  TupleSchema tuple_schema;
  tuple_schema.append_cell(TupleCellSpec("", "Tables_in_SYS", "Tables_in_SYS"));
  sql_result->set_tuple_schema(tuple_schema);

  auto oper = new StringListPhysicalOperator;
  for (const std::string &s : all_tables) {
    oper->append(s);
  }

  sql_result->set_operator(std::unique_ptr<PhysicalOperator>(oper));
  return RC::SUCCESS;
}

RC ExecuteStage::do_desc_table(SQLStageEvent *sql_event)
{
  SqlResult *sql_result = new SqlResult;
  sql_event->session_event()->set_sql_result(sql_result);

  Command *cmd = sql_event->command().get();
  const char *table_name = cmd->desc_table.relation_name.c_str();

  Db *db = sql_event->session_event()->session()->get_current_db();
  Table *table = db->find_table(table_name);
  if (table != nullptr) {

    TupleSchema tuple_schema;
    tuple_schema.append_cell(TupleCellSpec("", "Field", "Field"));
    tuple_schema.append_cell(TupleCellSpec("", "Type", "Type"));
    tuple_schema.append_cell(TupleCellSpec("", "Length", "Length"));

    // TODO add Key
    sql_result->set_tuple_schema(tuple_schema);

    auto oper = new StringListPhysicalOperator;
    const TableMeta &table_meta = table->table_meta();
    for (int i = table_meta.sys_field_num(); i < table_meta.field_num(); i++) {
      const FieldMeta *field_meta = table_meta.field(i);
      oper->append({field_meta->name(), attr_type_to_string(field_meta->type()), std::to_string(field_meta->len())});
    }

    sql_result->set_operator(std::unique_ptr<PhysicalOperator>(oper));
  } else {

    sql_result->set_return_code(RC::SCHEMA_TABLE_NOT_EXIST);
    sql_result->set_state_string("Table not exists");
  }
  return RC::SUCCESS;
}

RC ExecuteStage::do_insert(SQLStageEvent *sql_event)
{
  SessionEvent *session_event = sql_event->session_event();
  SqlResult *sql_result = new SqlResult;
  session_event->set_sql_result(sql_result);

  Session *session = session_event->session();
  Db *db = session->get_current_db();

  Trx *trx = session->current_trx();
  CLogManager *clog_manager = db->get_clog_manager();

  Stmt *stmt = sql_event->stmt();
  if (stmt == nullptr) {
    LOG_WARN("cannot find statement");
    return RC::GENERIC_ERROR;
  }

  InsertStmt *insert_stmt = (InsertStmt *)stmt;
  Table *table = insert_stmt->table();

  RC rc = table->insert_record(trx, insert_stmt->value_amount(), insert_stmt->values());
  if (rc == RC::SUCCESS) {
    if (!session->is_trx_multi_operation_mode()) {

      CLogRecord *clog_record = nullptr;
      rc = clog_manager->clog_gen_record(CLogType::REDO_MTR_COMMIT, trx->get_current_id(), clog_record);
      if (rc != RC::SUCCESS || clog_record == nullptr) {
        if (rc == RC::SUCCESS) {
          rc = RC::INTERNAL;
        }
        sql_result->set_return_code(rc);
        return rc;
      }

      rc = clog_manager->clog_append_record(clog_record);
      if (rc != RC::SUCCESS) {
        sql_result->set_return_code(rc);
        return rc;
      }

      trx->next_current_id();
      sql_result->set_return_code(RC::SUCCESS);
    } else {
      sql_result->set_return_code(RC::SUCCESS);
    }
  } else {
    sql_result->set_return_code(rc);
  }
  return rc;
}

RC ExecuteStage::do_delete(SQLStageEvent *sql_event)
{
#if 0
  SessionEvent *session_event = sql_event->session_event();
  Session *session = session_event->session();

  Db *db = session->get_current_db();
  Trx *trx = session->current_trx();
  CLogManager *clog_manager = db->get_clog_manager();

  Stmt *stmt = sql_event->stmt();
  if (stmt == nullptr) {
    LOG_WARN("cannot find statement");
    return RC::GENERIC_ERROR;
  }

  DeleteStmt *delete_stmt = (DeleteStmt *)stmt;

  TableScanOperator scan_oper(delete_stmt->table());
  PredicateOperator pred_oper(delete_stmt->filter_stmt());
  pred_oper.add_child(&scan_oper);

  DeleteOperator delete_oper(delete_stmt, trx);
  delete_oper.add_child(&pred_oper);

  RC rc = delete_oper.open();
  if (rc != RC::SUCCESS) {
    session_event->set_response("FAILURE\n");
  } else {

    session_event->set_response("SUCCESS\n");
    if (!session->is_trx_multi_operation_mode()) {

      CLogRecord *clog_record = nullptr;
      rc = clog_manager->clog_gen_record(CLogType::REDO_MTR_COMMIT, trx->get_current_id(), clog_record);
      if (rc != RC::SUCCESS || clog_record == nullptr) {
        session_event->set_response("FAILURE\n");
        return rc;
      }

      rc = clog_manager->clog_append_record(clog_record);
      if (rc != RC::SUCCESS) {
        session_event->set_response("FAILURE\n");
        return rc;
      } 

      trx->next_current_id();
      session_event->set_response("SUCCESS\n");
    }
  }
  return rc;
#endif
  return RC::SUCCESS;
}

RC ExecuteStage::do_begin(SQLStageEvent *sql_event)
{
  SessionEvent *session_event = sql_event->session_event();
  SqlResult *sql_result = new SqlResult;
  session_event->set_sql_result(sql_result);

  Session *session = session_event->session();
  Db *db = session->get_current_db();
  Trx *trx = session->current_trx();
  CLogManager *clog_manager = db->get_clog_manager();

  session->set_trx_multi_operation_mode(true);

  CLogRecord *clog_record = nullptr;
  RC rc = clog_manager->clog_gen_record(CLogType::REDO_MTR_BEGIN, trx->get_current_id(), clog_record);
  if (rc != RC::SUCCESS || clog_record == nullptr) {
    sql_result->set_return_code(rc);
    return rc;
  }

  rc = clog_manager->clog_append_record(clog_record);
  sql_result->set_return_code(rc);

  return rc;
}

RC ExecuteStage::do_commit(SQLStageEvent *sql_event)
{

  SessionEvent *session_event = sql_event->session_event();
  SqlResult *sql_result = new SqlResult;
  session_event->set_sql_result(sql_result);

  Session *session = session_event->session();
  session->set_trx_multi_operation_mode(false);

  Trx *trx = session->current_trx();
  CLogRecord *clog_record = nullptr;

  Db *db = session->get_current_db();
  CLogManager *clog_manager = db->get_clog_manager();
  RC rc = clog_manager->clog_gen_record(CLogType::REDO_MTR_COMMIT, trx->get_current_id(), clog_record);
  if (rc != RC::SUCCESS || clog_record == nullptr) {
    sql_result->set_return_code(rc);
    return rc;
  }

  rc = clog_manager->clog_append_record(clog_record);
  sql_result->set_return_code(rc);

  trx->next_current_id();

  return rc;
}

RC ExecuteStage::do_clog_sync(SQLStageEvent *sql_event)
{

  SqlResult *sql_result = new SqlResult;
  SessionEvent *session_event = sql_event->session_event();
  session_event->set_sql_result(sql_result);

  Db *db = session_event->session()->get_current_db();
  CLogManager *clog_manager = db->get_clog_manager();

  RC rc = clog_manager->clog_sync();

  sql_result->set_return_code(rc);

  return rc;
}