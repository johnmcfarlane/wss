find_package(fmt REQUIRED)
find_package(lyra REQUIRED)

add_executable(rack main.cpp)
target_link_libraries(
  rack
  all_lexicon
  bfg::lyra
  common
  fmt::fmt
  scrabble_lexicon
  wwf_lexicon)
set_target_properties(rack PROPERTIES CXX_STANDARD 20)

add_test(NAME rack COMMAND rack abc)
