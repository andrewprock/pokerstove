project(eval)

add_executable(ps-eval main.cc)

target_link_libraries(ps-eval
        peval
        penum
        ${Boost_LIBRARIES}
)
