project(eval)

add_executable(ps-colex main.cpp)

target_link_libraries(ps-colex
        peval
        ${Boost_LIBRARIES}
)
