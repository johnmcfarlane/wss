cmake_minimum_required(VERSION 3.16.3)
project(wss)

include(CTest)

add_subdirectory(src)
add_subdirectory(test/approval)

install(TARGETS wordle-main EXPORT WssTargets)
