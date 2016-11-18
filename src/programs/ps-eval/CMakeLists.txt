project(eval)

include(CheckCXXCompilerFlag)

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

add_executable(ps-eval main.cpp)
#add_definitions ("-ansi -Wall -std=c++0x")
add_definitions ("-std=c++0x")

target_link_libraries(ps-eval
        peval
        penum
        ${Boost_LIBRARIES}
)
