project(eval)

add_executable(ps-eval main.cpp)
#add_definitions ("-ansi -Wall -std=c++0x")
add_definitions ("-std=c++0x")

target_link_libraries(ps-eval
        peval
        ${Boost_LIBRARIES}
)
