project(eval)

add_executable(ps-lut main.cpp)
add_definitions ("-std=c++11")

target_link_libraries(ps-lut
        peval
        ${Boost_LIBRARIES}
)
