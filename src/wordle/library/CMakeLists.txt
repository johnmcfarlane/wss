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

# Wordle library tests
add_executable(wordle-constraints-test constraints-test.cpp)
target_link_libraries(wordle-constraints-test PRIVATE wordle unit-test)
add_test(wordle/constraints wordle-constraints-test)

add_executable(wordle-generate_constraints-test generate_constraints-test.cpp)
target_link_libraries(wordle-generate_constraints-test PRIVATE wordle unit-test)
add_test(wordle/generate_constraints wordle-generate_constraints-test)

add_executable(wordle-parse_command_line-test parse_command_line-test.cpp)
target_link_libraries(wordle-parse_command_line-test PRIVATE wordle unit-test)
add_test(wordle/parse_command_line wordle-parse_command_line-test)

add_executable(wordle-run-test run-test.cpp)
target_link_libraries(wordle-run-test PRIVATE wordle unit-test)
add_test(wordle/run wordle-run-test)

add_executable(wordle-solve-test solve-test.cpp)
target_link_libraries(wordle-solve-test PRIVATE wordle unit-test)
add_test(wordle/solve wordle-solve-test)

add_executable(wordle-word-test word-test.cpp)
target_link_libraries(wordle-word-test PRIVATE wordle unit-test)
add_test(wordle/word wordle-word-test)
