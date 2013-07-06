cmake_minimum_required(VERSION 2.6)

project(pokerstove)

#add_definitions ("-ansi -pedantic -Wall -Wextra")
add_definitions ("-ansi -Wall")

# Set up gtest. This must be set up before any subdirectories are
# added which will use gtest.
add_subdirectory(ext/gtest)
find_library(gtest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
link_directories(${GTEST_LIBS_DIR})
add_definitions ("-fPIC")

add_subdirectory(pokerstove/peval)

