# miniOB泛读报告

# 1.    软件系统中的类及其功能

基于对“miniOB”开源软件的代码的阅读，以及对其代码组织结构和功能的分析，“miniOB”的代码构成和每个模块对应的主要功能如表1所示。



**表1** **miniOB的代码构成及实现功能**

| **程序包** | **子包** | **类**                | **实现功能**               |
| ---------- | -------- | --------------------- | -------------------------- |
| storage    | clog               | CLogRecord                                         | 处理日志记录 ，记录历史                                      |
|            |                    | CLogBuffer                                         | 管理缓存的日志记录                                           |
|            |                    | CLogFile                                           | 管理日志文件                                                 |
|            |                    | CLogManager                                        | 管理日志记录                                                 |
|            | index              | BplusTreeIndex                                     | 管理和操作B+树索引                                           |
|            |                    | BplusTreeIndexScanner                              | 扫描B+树索引                                                 |
|            |                    | AttrComparator                                     | 比较两个属性                                                 |
|            |                    | KeyComparator                                      | 根据属性值和RID值比较两个关键字，从而提供排序功能            |
|            |                    | AttrPrinter                                        | 根据属性类型将值转换成字符串                                 |
|            |                    | KeyPrinter                                         | 将键值对转换成字符串表示的功能，包括属性值和RID值            |
|            |                    | IndexFileHeader                                    | 存储与数据文件的结构相关的属性、键、类型和根页号信息的成员，还包含了将这些信息转换为字符串格式以及设置不同成员值的方法 |
|            |                    | IndexNodeHandler                                   | 初始化、验证和检索数据文件中索引节点信息                     |
|            |                    | LeafIndexNodeHandler : public IndexNodeHandler     | 检索B+树叶节点的信息                                         |
|            |                    | InternalIndexNodeHandler : public IndexNodeHandler | 操作B+树非叶节点的信息                                       |
|            |                    | BplusTreeHandler                                   | 提供创建索引、打开索引、插入索引项、删除索引项、获取记录等操作，支持对B+树结构进行打印输出和验证 |
|            |                    | BplusTreeScanner                                   | 用于在指定范围内扫描数据，并返回RID(记录标识符)。该类提供了open打开扫描器，next_entry获取下一条数据，以及close关闭扫描器的方法 |
|            |                    | Index                                              | 数据索引                                                     |
|            |                    | IndexScanner                                       | 遍历元素数据                                                 |
|            | common             | IndexMeta                                          | 元数据与JSON格式的互相转换                                   |
|            |                    | ConditionFilter                                    | 匹配一条数据的基类                                           |
|            |                    | DefaultConditionFilter : public ConditionFilter    | 匹配的具体实现                                               |
|            |                    | CompositeConditionFilter : public ConditionFilter  | 匹配的具体实现                                               |
|            |                    | Db                                                 | 数据库类，包括初始化、创建表、查找表、列出所有表、同步、恢复和获取操作日志管理器等方法 |
|            |                    | FieldMeta                                          | 定义字段的基本属性，如名称、类型、偏移量、长度等，以及对这些属性进行初始化、输出描述和输出JSON格式 |
|            |                    | Field                                              | 维护Table和FieldMeta                                         |
|            |                    | Table                                              | 表类，维护元数据保存的文件(完整路径)、表名、表数据存放的路径、字段个数、字段、clog管理器，用于维护redo log |
|            | default            | DefaultHandler                                     | 管理和处理数据库和表                                         |
|            |                    | DefaultStorageStage                                | 处理事件和回调的函数的实现                                   |
|            |                    | Frame                                              | 描述数据帧                                                   |
|            |                    | BPFrameId                                          | 表示缓冲池中帧的唯一标识符                                   |
|            |                    | BPFrameManager                                     | 管理缓冲池中的帧                                             |
|            |                    | BufferPoolIterator                                 | 在缓冲池中迭代页面                                           |
|            |                    | DiskBufferPool                                     | 管理分页文件的缓冲池                                         |
|            |                    | BufferPoolManager                                  | 管理缓冲池                                                   |
|            | record             | RecordPageHandler                                  | 管理存储记录的页                                             |
|            |                    | RecordFileHandler                                  | 管理存储记录的文件                                           |
|            |          | RecordFileScanner | 对记录文件的扫描 |
|            |                    | Record                                             | 表示记录                                                     |
|            |                    | RecordPageIterato                                  | 管理记录页的迭代器                                           |
|            | mem                | MemStorageStage                                    | 实现内存存储阶段的功能                                       |
|            | trx                | Operation                                          | 对记录的操作                                                 |
|            |          | OperationHasher | 为操作提供哈希函数实现 |
|            |          | OperationEqualer | 为操作提供相等比较函数实现 |
|            |                    | Trx                                                | 实现事务的相关操作                                           |
|            | persist            | PersistHandler                                     | 提供对文件的基本操作                                         |
| event      | optimize_event     | OptimizeEvent                                      | 维护一个指向SQLStageEvent对象的指针和一个指向common::StageEvent对象的指针 |
|            | session_event      | SessionEvent                                       | 包含了与会话相关的属性和函数，比如获取和设置通信器、会话、查询、SQL结果、响应等 |
|            | sql_event          | SQLStageEvent                                      | 表示与SQL执行相关的阶段事件，包括SQL语句、命令、语句、物理运算符等属性。同时提供了对这些属性的设置和获取方法 |
|            | storage_event | StorageEvent | 初始化一个值为SQLStageEvent类型的参数sql_event_ |
| net | communicator | Communicator | 负责与客户端通讯 |
|            |          | PlainCommunicator | 与客户端进行通讯 |
|            |          | CommunicateProtocol | 当前支持的通讯协议 |
|            |                    | CommunicatorFactory                                | 新建Communicator对象                                         |
|            | mysql_communicator | MysqlCommunicator : public Communicator | 与客户端通讯, 实现MySQL通讯协议 |
|            | server_param | ServerParam | 服务端参数 |
|            | server | Server | 实现服务端 |
| session | session_stage | SessionStage | stage是一个事件处理的几个阶段,包括session,parse,execution和storage, 每个stage使用handleEvent函数处理任务，并且使用StageEvent::pushCallback注册回调函数。这时当调用StageEvent::done(Immediate)时，就会调用该事件注册的回调函数。 |
|            | session | Session | 事务类 |
| sql | executor           | ExecuteStage                                       | 实现建表、建索引、展示表、表排序、增删改查等操作             |
|            |                    | SqlResult                                          | 用于存储SQL执行结果的类，可以设置元组模式、返回码、状态字符等信息，并提供了查询操作的接口，其中查询操作包括打开查询、关闭查询以及获取下一条元组 |
|            | expr               | Expression                                         | 表达式的抽象描述,比如获取某个字段的值、比较运算、类型转换    |
|            |                    | FieldExpr : public Expression                      | 用于表示数据库查询中的字段表达式。存储了某个表格中特定字段的信息，并提供了访问其元数据以及从元组中检索值的方法 |
|            |                    | ValueExpr : public Expression                      | 定义一个值表达式类，用于处理不同数据类型的值，将数据类型转换为内部实现使用的TupleCell类型，支持从元组中获取该值表达式的值 |
|            |                    | CastExpr : public Expression                       | 表示数据类型转换表达式，它可以将一个子表达式转换为指定的数据类型。其中包含一个子表达式和要转换的数据类 |
|            |                    | ComparisonExpr : public Expression                 | 用于表示比较表达式的节点，包括了比较运算符、左右子表达式、类型、取值等成员函数 |
|            |                    | ConjunctionExpr : public Expression                | 多个表达式使用同一种关系(AND或OR)来联结， 当前miniob仅有AND操作 |
|            |                    | TupleCellSpec                                      | 表示一个元组中的一个单元格的规范，包括表名、字段名和别名等信息，并提供了获取这些信息的方法 |
|            |                    | TupleCell                                          | 表示tuple中某个元素的值                                      |
|            |                    | TupleSchema                                        | 定义元组的结构信息。它提供了三个方法以添加元组中的单元格信息，一个方法用于获取元组中单元格的数量，一个方法用于按索引获取元组中的单元格信息。 |
|            |                    | Tuple                                              | 元组虚基类                                                   |
|            |                    | RowTuple : public Tuple                            | 表示数据库中的一行元组，并提供了设置其记录和模式、获取其单元格数量和内容以及通过表名和字段名查找特定单元格的方法，还管理其字段表达式的内存。 |
|            |                    | ProjectTuple : public Tuple                        | 该类包含了一个存储TupleCell对象的向量cells_，并实现了设置单元格，返回单元格数目，访问单元格以及查找单元格的函数。 |
|            |                    | JoinedTuple : public Tuple                         | 将两个tuple合并为一个tuple，在join算子中使用                 |
|            | operator           |                                                    |                                                              |
|            |                    |                                                    |                                                              |
|            |                    |                                                    |                                                              |
|            |                    |                                                    |                                                              |
|            |                    |                                                    |                                                              |
|            |                    |                                                    |                                                              |



——————————————————  以下是未修改的模板部分 ——————————————



## 2. 软件体系结构图

基于对“小米便签”代码组织结构、每个程序包在软件系统中的地位和作用、不同程序包之间的关系等方面的理解，可以用UML的包图绘制出“小米便签”开源软件的体系结构（见图1）。它整体上是一个XX性的体系结构，由XX层、XX层、XX个层次构成，每个层次包含若干个程序包。相邻层次的程序包之间存在交互。

(1) XX层

位于体系结构的 XX   ， 由XX 包中的类加以实现。

(2) XX层

该层是小米便签系统体系结构的核心，用于连接XX层和YY层，起到XX的作用，其提供的主要功能和服务包括：定义软件的业务XX，接收XX层的请求，与模型ZZ层、PP层进行交互，将处理结果返回给XX层。该层次主要通过“bb”、 “ww”包和“gg”包中的“ee”和“rr”子包以及其他相关部件加以实现。

(3) hh层

该层负责对小米便签的单个便签项进行建模，提供了便签项的基本操作功能和服务，并与数据层进行交互，以支持便签的创建、访问和修改。该层主要通过“model”程序包中的“NX ”类、“WX”类等加以实现。

(4) DD层

该层负责对小米便签的相关数据进行组织和存储，提供数XXX等功能和服务。该层主要通过“data”和“gtask.data”程序包加以实现。

 

**图1.** **小米便签体系结构图**

## 3.    各个子系统的类图

针对“小米便签”开源代码的各个程序包，还可以进一步深入分析包中的类以及它们之间的关系，绘制其实现类图。

 

**图2. data****程序包的实现类图**

 

**图3. gtask.data****程序包的实现类图**

 

 

**图4. gtask.remote****程序包的实现类图**

 

**图5. model****程序包的实现类图**

 

**图6. widget****程序包的实现类图**

 

 

**图7. ui****程序包的实现类图**

 

## 4.    软件的功能描述

根据“小米便签”开源软件在GitHub托管网站上提供的说明，通过运行和操作该软件系统，可以掌握“小米便签”提供了如下一组功能。

(1) 编辑/XX/XX/XX便签，支持用户管理便签，具体包括：

\-    创建和编辑便签，将创建好的便签置于文件夹中

\-     

(2) 新建/XX/XX，支持用户管理便签文件夹，具体包括：

\-    创建一个

\-     

(3) 便签同步，实现与TTT 中备忘录进行同步，具体包括：

\-    将小米便签的事项上传到YYY 服务器

\-    将YYY 服务器上的事项下载到本地

(4) 便签搜索，支持  。

(5) 添加/删除WW，具体包括：

\-    增加一个

\-    可删除一个

(6) 识别便签中的电话号码和网址，对便签编辑页面上  。。。。

(7) 便签分享，将用户所创建的便签。。。

此外，小米便签软件还提供了其他一组。。等等。

## 5.    软件功能与类间的对应关系

| **序号** | **功能名称**     | **实现模块**      | **实现方法** |
| -------- | ---------------- | ----------------- | ------------ |
| 1        | 新建便签         | NoteEditActivity  |              |
| 2        | 删除便签         | NoteEditActivity  |              |
| 3        | 移动便签         | DataUtils         |              |
| 4        | 新建文件夹       | NotesListActivity |              |
| 5        | 查看文件夹       | NotesListActivity |              |
| 6        |                  |                   |              |
| 7        |                  |                   |              |
| 8        |                  |                   |              |
| 9        |                  |                   |              |
| 10       |                  |                   |              |
| 11       |                  |                   |              |
| 12       |                  |                   |              |
| 13       |                  |                   |              |
| 14       |                  |                   |              |
| 15       |                  |                   |              |
| 16       |                  |                   |              |
| 17       | 识别电话号码     | Contact           |              |
| 18       | 分享             | NoteEditActivity  |              |
| 19       | 保存便签（自动） | WorkingNote       |              |

## 6.    收获

ü 在进行小米便签运行环境配置

ü  

## 7.    存在的问题

ü 由于对Java语言和Andriod开发的架构不够熟悉，。。。加深理解；

ü 由于大部分工作都是通过自学进行的，。。。，需要在实践的过程中逐步提升。
