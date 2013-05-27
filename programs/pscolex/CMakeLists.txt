project(eval)

add_executable(pscolex main.cpp)
add_definitions ("-std=c++0x")

target_link_libraries(pscolex
        peval
        ${Boost_LIBRARIES}
)
