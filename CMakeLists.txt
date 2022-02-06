cmake_minimum_required(VERSION 3.16.3)
project(wss)

include(CTest)
add_test(NAME approval-tests
         COMMAND ${PROJECT_SOURCE_DIR}/test/bin/run-approval-tests)

add_subdirectory(src)

install(TARGETS play rack wordle EXPORT WssTargets)
