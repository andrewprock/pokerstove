project(eval)

add_executable(ps-lut main.cpp)

target_link_libraries(ps-lut
        peval
        ${Boost_LIBRARIES}
)
