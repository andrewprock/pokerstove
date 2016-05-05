cmake_minimum_required(VERSION 2.8)
find_package (Threads)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register")

# build the test 
file(GLOB test_sources *.test.cpp)
add_executable(util_tests ${test_sources})
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(PUBLIC util_test "-pthread")
endif()
target_link_libraries(util_tests peval gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

# there are no libraries, only headers

add_test(TestUtil ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/util_tests)
