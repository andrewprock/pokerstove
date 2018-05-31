project(eval)

add_executable(ps-lut main.cc)

target_link_libraries(ps-lut
        peval
        ${Boost_LIBRARIES}
)
