cmake_minimum_required(VERSION 2.8)
find_package (Threads)

# build the test
file(GLOB test_sources *_test.cc)
add_executable(penum_tests ${test_sources})
target_link_libraries(penum_tests penum peval gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

# build the library sources from all sources, removing the test sources
file(GLOB lib_sources *.cc)
foreach(source_file ${test_sources})
  list(REMOVE_ITEM lib_sources ${source_file})
endforeach()

# penum library
add_library(penum ${lib_sources})

add_test(TestPenum ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/penum_tests)
