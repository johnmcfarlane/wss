cmake_minimum_required(VERSION 3.10.2)
project(wss)

set(CONAN_SYSTEM_INCLUDES ON)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)

add_subdirectory(src/common)

add_subdirectory(src/generate)
add_subdirectory(src/lexicon)
add_subdirectory(src/play)
add_subdirectory(src/rack)
