find_package(fmt REQUIRED)
find_package(lyra REQUIRED)
find_package(Microsoft.GSL REQUIRED)

add_executable(
  play
  board_premiums.cpp
  board_tiles.cpp
  grid.cpp
  load_buffer.cpp
  main.cpp
  search.cpp
  solve.cpp)
target_link_libraries(
  play
  PRIVATE all_lexicon
          bfg::lyra
          common
          fmt::fmt
          Microsoft.GSL::GSL
          scrabble_lexicon
          wwf_lexicon)
