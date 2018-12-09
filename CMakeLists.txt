cmake_minimum_required(VERSION 3.13)
project(wss)

set(CMAKE_CXX_STANDARD 17)

add_executable(wss main.cpp trie.cpp words.cpp)
target_compile_options(wss PRIVATE "-Wall" "-Wextra" "-Werror")
set_target_properties(wss PROPERTIES CXX_STANDARD 17)

# ASan
#target_compile_options(wss PRIVATE "-lib=libc++")
#target_compile_options(wss PRIVATE "-fsanitize=address")
#target_link_options(wss PRIVATE "-lasan")

# UBSan
#target_compile_options(wss PRIVATE "-fsanitize=undefined")
#target_link_options(wss PRIVATE "-lubsan")

# MSan
#target_compile_options(wss PRIVATE "-fsanitize=memory" "-stdlib=libstdc++" "-fno-omit-frame-pointer")
#target_link_options(wss PRIVATE "-lstdc++" "-fsanitize=memory")

