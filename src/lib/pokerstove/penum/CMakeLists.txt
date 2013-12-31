project(libpenum)

cmake_minimum_required(VERSION 2.8)

# build the library sources from all sources, removing the test sources
file(GLOB lib_sources *.cpp)
foreach(source_file ${test_sources})
list(REMOVE_ITEM lib_sources ${source_file})
endforeach()
add_library(penum ${lib_sources})

# build the test
#file(GLOB test_sources *.test.cpp)
#add_executable(penum_tests ${test_sources})
#find_package (Threads)
#target_link_libraries(penum_tests penum peval gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})
#add_test(TestPenum penum_tests )
