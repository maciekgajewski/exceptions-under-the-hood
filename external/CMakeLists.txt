# build google test

set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)
add_subdirectory(googletest)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_INSTALL OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
add_subdirectory(googlebenchmark)
