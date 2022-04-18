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
