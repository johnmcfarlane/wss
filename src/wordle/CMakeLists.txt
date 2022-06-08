add_subdirectory(library)

# Wordle program
add_executable(wordle-main main.cpp)
target_link_libraries(wordle-main PRIVATE common wordle)
set_target_properties(wordle-main PROPERTIES OUTPUT_NAME wordle)

add_subdirectory(functional-tests)
