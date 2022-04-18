add_subdirectory(library)

# Wordle program
add_executable(wordle-main main.cpp)
target_link_libraries(wordle-main PRIVATE common wordle)
set_target_properties(wordle-main PROPERTIES OUTPUT_NAME wordle)

# Wordle program tests
add_test(NAME wordle-main COMMAND wordle)
add_test(NAME wordle-main-help COMMAND wordle --help)
