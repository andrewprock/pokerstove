project(eval)

add_executable(eval main.cpp)
add_definitions ("-ansi -Wall -std=c++0x")

target_link_libraries(eval
        peval
        boost_program_options
)
