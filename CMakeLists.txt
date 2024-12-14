cmake_minimum_required(VERSION 3.5)

project(
    pokerstove
    VERSION "1.2"
    LANGUAGES CXX)

#include(GNUInstallDirs)

# set up some global compiler opions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register")

add_definitions("-fPIC")
enable_testing()

#
# set up boost
#
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost CONFIG COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
message(STATUS "Boost Include: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost Libraries: ${Boost_LIBRARIES}")

# Also search for includes in PROJECT_BINARY_DIR to find config.h.
include_directories("${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


# add the actual code
include_directories(src/lib)
add_subdirectory(src/lib)

# This controls whether or not we are building the python
# binaries, and possibly the wheel file.
# TODO: add a separate option for building the wheel vs. python
if(BUILD_PYTHON OR BUILD_WHEEL)
    add_subdirectory(src/lib/python)
endif()

if(NOT BUILD_WHEEL)
    # none of these are needed in the wheel file
    add_subdirectory(src/programs)
    add_subdirectory(src/ext/googletest)
    # Add make install functionality
    install(TARGETS peval)
    install(TARGETS penum)
    install(TARGETS ps-colex)
    install(TARGETS ps-eval)
    install(TARGETS ps-lut)
endif()
