# build google test

set(BUILD_GMOCK OFF CACHE BOOL "")
set(INSTALL_GTEST OFF CACHE BOOL "")
add_subdirectory(googletest)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "")
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "")
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "")
add_subdirectory(googlebenchmark)
