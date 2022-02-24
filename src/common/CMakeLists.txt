add_library(common letter_set.cpp letter_set.h scores.h ssize.h tile.h)
set_target_properties(common PROPERTIES CXX_STANDARD 17)
target_link_libraries(common PRIVATE "${CONAN_LIBS}")
target_include_directories(common INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
