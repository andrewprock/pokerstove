cmake_minimum_required(VERSION 2.6)

project(pokerstove)

# Set up gtest. This must be set up before any subdirectories are
# added which will use gtest.
add_subdirectory(src/ext/googletest)
find_library(gtest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
link_directories(${GTEST_LIBS_DIR})
add_definitions("-fPIC")
enable_testing()

#
# set up boost
#
# The goal is to make this work for linux/win/osx
if (UNIX)
    message(status "building for unix")
    set(BOOST_LIBRARYDIR "/usr/local/lib/")
elseif(CMAKE_GENERATOR STREQUAL Xcode)
    message(status ": building for xcode")
    set(BOOST_ROOT "/opt/include/")
    set(BOOST_LIBRARYDIR "/opt/local/lib")
else()
    message(status ": building for non-unix -> windows")
    set(BOOST_ROOT "E:/code/ext/boost_1_53_0/")
    set(BOOST_LIBRARYDIR "E:/code/ext/boost_1_53_0/lib32-msvc-10.0/")
endif()
set(Boost_USE_STATIC_LIBS   OFF)
set(Boost_USE_MULTITHREADED      ON)
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
message(status "** Boost Libraries: ${Boost_LIBRARIES}")

# Also search for includes in PROJECT_BINARY_DIR to find config.h.
include_directories("${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(src)
