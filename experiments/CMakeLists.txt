add_executable(int2uint int2uint.cc)
add_executable(constint constint.cc)

add_executable(exception_test_compiled_in
  exception_tests.cc exception_types.cc exception_types.hh)

add_library(exception_types SHARED
  exception_types.cc exception_types.hh)

add_executable(exception_test_dl exception_tests.cc)
target_link_libraries(exception_test_dl exception_types)
