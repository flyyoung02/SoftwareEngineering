if(EXISTS "/root/miniob/build/unitest/clog_test[1]_tests.cmake")
  include("/root/miniob/build/unitest/clog_test[1]_tests.cmake")
else()
  add_test(clog_test_NOT_BUILT clog_test_NOT_BUILT)
endif()
