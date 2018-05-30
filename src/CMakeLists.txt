# check_function_exists does not work for builtins, so use try_compile.
try_compile(HAVE_BUILTIN_BITOPS
            "${PROJECT_BINARY_DIR}"
            "${CMAKE_CURRENT_SOURCE_DIR}/TestBuiltinBitops.cpp")

# Set up the configure file.
configure_file(Config.h.in Config.h)

include_directories(lib/)
add_subdirectory(lib/pokerstove/peval)
add_subdirectory(lib/pokerstove/penum)
add_subdirectory(lib/pokerstove/util)
add_subdirectory(programs/ps-colex)
add_subdirectory(programs/ps-eval)
add_subdirectory(programs/ps-lut)
