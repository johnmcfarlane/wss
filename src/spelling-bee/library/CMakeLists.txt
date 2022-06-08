find_package(fmt REQUIRED)
find_package(lyra REQUIRED)
find_package(Microsoft.GSL REQUIRED)

# Spelling Bee library
add_wss_library(spelling-bee parse_command_line.cpp run.cpp solve.cpp)
target_link_libraries(
  spelling-bee
  PUBLIC common fmt::fmt
  PRIVATE bfg::lyra scrabble_lexicon
)
target_include_directories(common PUBLIC include)

# Spelling Bee library unit tests
add_unit_test(
  spelling-bee-parse_command_line.unit-test parse_command_line.unit-test.cpp
  spelling-bee
)
add_unit_test(spelling-bee-run.unit-test run.unit-test.cpp spelling-bee)
add_unit_test(spelling-bee-solve.unit-test solve.unit-test.cpp spelling-bee)

# Spelling Bee library fuzz tests
add_fuzz_test(
  spelling-bee-parse_command_line-fuzz-test parse_command_line.fuzz-test.cpp
  spelling-bee
)
add_fuzz_test(spelling-bee-run-fuzz-test run.fuzz-test.cpp spelling-bee)
add_fuzz_test(spelling-bee-solve-fuzz-test solve.fuzz-test.cpp spelling-bee)
