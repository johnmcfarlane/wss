find_package(fmt REQUIRED)
find_package(lyra REQUIRED)
find_package(Microsoft.GSL REQUIRED)

# Wordle library
add_library(wordle generate_constraints.cpp parse_command_line.cpp run.cpp
                   solve.cpp)
target_link_libraries(
  wordle
  PUBLIC common fmt::fmt
  PRIVATE bfg::lyra wordle_lexicon)
target_include_directories(common PUBLIC include)

# Wordle library unit tests
add_unit_test(wordle-constraints-unit-test constraints-test.cpp wordle)
add_unit_test(wordle-generate_constraints-unit-test
              generate_constraints-test.cpp wordle)
add_unit_test(wordle-parse_command_line-unit-test parse_command_line-test.cpp
              wordle)
add_unit_test(wordle-run-unit-test run-test.cpp wordle)
add_unit_test(wordle-solve-unit-test solve-test.cpp wordle)
add_unit_test(wordle-word-unit-test word-test.cpp wordle)
