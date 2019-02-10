set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(expression_parser_test test.cc)
target_link_libraries(expression_parser_test gtest)
