add_library(common letter_set.cpp letter_set.h scores.h ssize.h tile.h)
set_target_properties(common PROPERTIES CXX_STANDARD 20)
target_include_directories(common INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
