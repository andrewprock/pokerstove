project(eval)

add_executable(pseval main.cpp)
#add_definitions ("-ansi -Wall -std=c++0x")
add_definitions ("-std=c++0x")

target_link_libraries(pseval
        peval
        ${Boost_LIBRARIES}
)
