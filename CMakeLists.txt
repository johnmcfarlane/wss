cmake_minimum_required(VERSION 3.16.3)
project(wss)

include(CTest)

add_subdirectory(src)

install(TARGETS wordle-main EXPORT WssTargets)
