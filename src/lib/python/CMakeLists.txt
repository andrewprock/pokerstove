cmake_minimum_required(VERSION 3.18)

# find swig and python and configure them
set(CMAKE_SWIG_FLAGS)
set(UseSWIG_MODULE_VERSION 2)
find_package(SWIG REQUIRED)
include(UseSWIG)

if(UNIX AND NOT APPLE)
  list(APPEND CMAKE_SWIG_FLAGS "-DSWIGWORDSIZE64")
endif()
find_package(Python3 REQUIRED COMPONENTS Interpreter Development.Module)
list(APPEND CMAKE_SWIG_FLAGS "-DPY3")

### wrapping code below

set(PYTHON_PROJECT pokerstove)
set(PYTHON_PROJECT_DIR ${PROJECT_BINARY_DIR}/python/${PYTHON_PROJECT})
message(STATUS "Python project: ${PYTHON_PROJECT}")
message(STATUS "Python project build path: ${PYTHON_PROJECT_DIR}")

# I'm not sure if we can use peval for a target name.
# Use eval instead, or maybe pseval if that doesn't work.
set(PS_TARGET pokerstove)
set_property(SOURCE pokerstove.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE pokerstove.i PROPERTY SWIG_MODULE_NAME ${PS_TARGET})
swig_add_library(${PS_TARGET}
  TYPE MODULE
  LANGUAGE python
  SOURCES pokerstove.i)
add_library(${PROJECT_NAMESPACE}::${PS_TARGET} ALIAS ${PS_TARGET})

install(TARGETS ${PS_TARGET} DESTINATION . COMPONENT python)
get_property(_support_files TARGET ${PS_TARGET} PROPERTY SWIG_SUPPORT_FILES)
install(FILES "${_support_files}" DESTINATION . COMPONENT python)

# note: macOS is APPLE and also UNIX !
if(APPLE)
  set_property(TARGET ${PS_TARGET} APPEND PROPERTY
    LINK_FLAGS "-flat_namespace -undefined suppress"
    )
endif()


# This is done to ensure that the .py wrapper and the .so file are in the
# same location so that a properly set PYTHONPATH will pick them up.
# TODO: use standard packaging to support proper installation of python
# TODO: extension
set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/python/${PYTHON_PROJECT})

message(STATUS "PYTHON_PROJECT ${PYTHON_PROJECT_DIR}")
message(STATUS "swig_add_library OUTPUT_DIR ${PYTHON_PROJECT_DIR}/${PS_TARGET}")

file(GENERATE OUTPUT ${PYTHON_PROJECT_DIR}/__init__.py
     CONTENT "__version__ = \"${PROJECT_VERSION}\"\nfrom .pokerstove import *\n")

# This file is nested within the library source dir, so
# look to the parent directory for the include files. The
# python includes should be in the standard place.
target_include_directories(${PS_TARGET} PRIVATE ..  ${Python3_INCLUDE_DIRS})
set_property(TARGET ${PS_TARGET} PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON)
target_link_libraries(${PS_TARGET} PRIVATE peval penum)

