cmake_minimum_required(VERSION 2.8)
find_package (Threads)


include(CheckCXXCompilerFlag)




set (CMAKE_CXX_FLAGS "-Wno-deprecated-register")


if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)
    if (HAS_CPP14_FLAG)
        message(status ": has c++14")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    elseif (HAS_CPP11_FLAG)
        message(status ": has c++11")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        message(FATAL_ERROR "Unsupported compiler -- pokerstove  requires C++11 support!")
    endif()
endif()



# build the test 
file(GLOB test_sources *.test.cpp)
add_executable(peval_tests ${test_sources})
target_link_libraries(peval_tests peval gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

# build the library sources from all sources, removing the test sources
file(GLOB lib_sources *.cpp)
foreach(source_file ${test_sources})
  list(REMOVE_ITEM lib_sources ${source_file})
endforeach()

# peval library
add_library(peval ${lib_sources})

add_test(TestPeval ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/peval_tests)
