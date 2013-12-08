project(eval)

add_executable(boardevallut main.cpp)
add_definitions ("-std=c++11")

target_link_libraries(boardevallut
        peval
        ${Boost_LIBRARIES}
)
