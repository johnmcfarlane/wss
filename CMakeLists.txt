cmake_minimum_required(VERSION 3.13)
project(wss)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src/common)

add_subdirectory(src/generate)
add_subdirectory(src/lexicon)
add_subdirectory(src/play)
add_subdirectory(src/rack)
