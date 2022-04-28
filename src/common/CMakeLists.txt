find_package(Microsoft.GSL REQUIRED)
find_package(fmt REQUIRED)

add_library(common letter_set.cpp letter_set.h open_file.cpp scores.h tile.h)
target_link_libraries(
  common
  PUBLIC Microsoft.GSL::GSL
  PRIVATE fmt::fmt)
target_compile_features(common PUBLIC cxx_std_20)
target_include_directories(common INTERFACE "${CMAKE_CURRENT_LIST_DIR}")

# common library unit tests
add_unit_test(common-letter_set-unit-test letter_set-test.cpp wordle)
