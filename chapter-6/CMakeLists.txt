cmake_minimum_required(VERSION 3.28)
project(jp-engine-6)

add_subdirectory(engine)
add_subdirectory(game)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if (CLANG_TIDY_EXE)
    set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}")

    set_target_properties(engine-6 PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    set_target_properties(game-6 PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
else()
    message("Clang Tidy not found")
endif()