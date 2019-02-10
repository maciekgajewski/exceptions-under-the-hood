set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(expression_parser
  utils.cc utils.hh
  evaluate_exceptions.cc evaluate_exceptions.hh)

add_executable(expression_parser_test test.cc)
target_link_libraries(expression_parser_test gtest)
target_link_libraries(expression_parser_test expression_parser)

