cmake_minimum_required(VERSION 2.6)

project(programs)

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
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED      ON)
find_package(Boost COMPONENTS program_options REQUIRED)

message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
message(status "** Boost Libraries: ${Boost_LIBRARIES}")

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

include_directories(../libs)

enable_testing()

add_subdirectory(../libs libs)
add_subdirectory (pseval)
add_subdirectory (pscolex)
#add_subdirectory (omahalut)