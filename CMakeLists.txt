cmake_minimum_required(VERSION 3.13)
project(wss)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)

set(CMAKE_CXX_STANDARD 17)

add_executable(scrabble src/scrabble.cpp src/trie.cpp src/words.cpp)
target_compile_options(scrabble PRIVATE "-Wall" "-Wextra" "-Werror" "-Wpedantic")
set_target_properties(scrabble PROPERTIES CXX_STANDARD 17)
target_link_libraries(scrabble "${CONAN_LIBS}")
