cmake_minimum_required(VERSION 2.6)

project(programs)

set (BOOST_LIBRARYDIR "/usr/local/lib/")
add_subdirectory(../libs libs)
include_directories(../libs)
add_subdirectory (pseval)