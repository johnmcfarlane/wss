add_subdirectory(library)

# Spelling Bee program
add_executable(spelling-bee-main main.cpp)
target_link_libraries(spelling-bee-main PRIVATE common spelling-bee)
set_target_properties(spelling-bee-main PROPERTIES OUTPUT_NAME spelling-bee)

add_subdirectory(functional-tests)
