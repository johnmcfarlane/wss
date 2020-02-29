add_executable(generate
        load_lexicon.cpp main.cpp multi_trie.cpp)
target_link_libraries(generate common)
