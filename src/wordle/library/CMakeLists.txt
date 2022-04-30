find_package(fmt REQUIRED)
find_package(lyra REQUIRED)
find_package(Microsoft.GSL REQUIRED)

# Wordle library
add_wss_library(wordle generate_constraints.cpp parse_command_line.cpp run.cpp
                solve.cpp)
target_link_libraries(
  wordle
  PUBLIC common fmt::fmt
  PRIVATE bfg::lyra wordle_lexicon)
target_include_directories(common PUBLIC include)

# Wordle library unit tests
add_unit_test(wordle-constraints.unit-test constraints.unit-test.cpp wordle)
add_unit_test(wordle-generate_constraints.unit-test
              generate_constraints.unit-test.cpp wordle)
add_unit_test(wordle-parse_command_line.unit-test
              parse_command_line.unit-test.cpp wordle)
add_unit_test(wordle-run.unit-test run.unit-test.cpp wordle)
add_unit_test(wordle-solve.unit-test solve.unit-test.cpp wordle)
add_unit_test(wordle-word.unit-test word.unit-test.cpp wordle)

# Wordle library fuzz tests
add_fuzz_test(wordle-generate_constraints-fuzz-test
              generate_constraints.fuzz-test.cpp wordle)
add_fuzz_test(wordle-parse_command_line-fuzz-test
              parse_command_line.fuzz-test.cpp wordle)
add_fuzz_test(wordle-run-fuzz-test run.fuzz-test.cpp wordle)
add_fuzz_test(wordle-solve-fuzz-test solve.fuzz-test.cpp wordle)
