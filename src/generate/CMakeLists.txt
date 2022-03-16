find_package(fmt REQUIRED)
find_package(lyra REQUIRED)

add_executable(generate load_lexicon.cpp main.cpp trie.cpp)
target_link_libraries(generate PRIVATE common fmt::fmt bfg::lyra)
set_target_properties(generate PROPERTIES CXX_STANDARD 20)
