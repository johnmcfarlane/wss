cmake_minimum_required(VERSION 3.16.3)
project(wss)

include(CTest)

add_subdirectory(src)

if(NOT ${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  add_subdirectory(test/approval)
endif()

install(TARGETS play rack wordle EXPORT WssTargets)
