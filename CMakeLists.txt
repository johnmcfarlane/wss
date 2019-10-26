cmake_minimum_required(VERSION 3.13)
project(wss)

set(CMAKE_CXX_STANDARD 17)

add_executable(scrabble scrabble.cpp trie.cpp words.cpp)
target_compile_options(scrabble PRIVATE "-Wall" "-Wextra" "-Werror" "-Wpedantic")
set_target_properties(scrabble PROPERTIES CXX_STANDARD 17)

# ASan
#target_compile_options(scrabble PRIVATE "-lib=libc++")
#target_compile_options(scrabble PRIVATE "-fsanitize=address")
#target_link_options(scrabble PRIVATE "-lasan")

# UBSan
#target_compile_options(scrabble PRIVATE "-fsanitize=undefined")
#target_link_options(scrabble PRIVATE "-lubsan")

# MSan
#target_compile_options(scrabble PRIVATE "-fsanitize=memory" "-stdlib=libstdc++" "-fno-omit-frame-pointer")
#target_link_options(scrabble PRIVATE "-lstdc++" "-fsanitize=memory")
