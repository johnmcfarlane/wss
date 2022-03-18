add_library(common letter_set.cpp letter_set.h scores.h tile.h)
target_compile_features(common PUBLIC cxx_std_20)
target_include_directories(common INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
