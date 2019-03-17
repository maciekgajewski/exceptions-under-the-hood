add_executable(int2uint int2uint.cc)

if(UNIX)
add_executable(constint constint.cc)
endif()

add_executable(exception_test_compiled_in
  exception_tests.cc exception_types.cc exception_types.hh)

add_library(exception_types SHARED
  exception_types.cc exception_types.hh)
set_target_properties(exception_types PROPERTIES COMPILE_FLAGS "-static-libstdc++")
set_target_properties(exception_types PROPERTIES LINK_FLAGS "-static-libstdc++")

add_executable(exception_test_dl exception_tests.cc)
target_link_libraries(exception_test_dl exception_types)
set_target_properties(exception_test_dl PROPERTIES COMPILE_FLAGS "-static-libstdc++")
set_target_properties(exception_test_dl PROPERTIES LINK_FLAGS "-static-libstdc++")


add_executable(terminate_handler terminate_handler.cc)

add_executable(sjlj_exceptions
  sjlj_exceptions.cc sjlj_exceptions.hh sjlj_exceptions_example.cc widget.hh widget.cc)
