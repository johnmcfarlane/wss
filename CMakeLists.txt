cmake_minimum_required(VERSION 3.13)
project(wss)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)

set(CMAKE_CXX_STANDARD 17)

add_executable(rack src/rack.cpp src/trie.cpp src/words.cpp)
target_compile_options(rack PRIVATE "-Wall" "-Wextra" "-Werror" "-Wpedantic")
set_target_properties(rack PROPERTIES CXX_STANDARD 17)
target_link_libraries(rack "${CONAN_LIBS}")
