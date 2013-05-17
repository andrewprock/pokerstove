cmake_minimum_required(VERSION 2.6)

project(programs)

set(BOOST_ROOT "E:/code/ext/boost_1_53_0/")
set(BOOST_LIBRARYDIR "E:/code/ext/boost_1_53_0/lib32-msvc-10.0/")
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(../libs)

add_subdirectory(../libs libs)
add_subdirectory (pseval)
