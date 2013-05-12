project(eval)

add_executable(pseval main.cpp)
add_definitions ("-ansi -Wall -std=c++0x")

target_link_libraries(pseval
        peval
        boost_program_options
)
