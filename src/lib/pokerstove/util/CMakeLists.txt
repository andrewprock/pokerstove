# build the test
file(GLOB test_sources *.test.cpp)
add_executable(util_tests ${test_sources})
target_link_libraries(util_tests peval gtest gtest_main)

# there are no libraries, only headers

add_test(TestUtil ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/util_tests)
