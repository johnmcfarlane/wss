find_package(Microsoft.GSL REQUIRED)
find_package(fmt REQUIRED)

# add a library target
function(add_wss_library name)
  add_library(${name} ${ARGN})
  target_compile_features(${name} PUBLIC cxx_std_20)
  target_compile_options(${name} PRIVATE ${WSS_FUZZER_FLAGS})
endfunction()

add_wss_library(common letter_set.cpp letter_set.h open_file.cpp scores.h
                tile.h)
target_link_libraries(
  common
  PUBLIC Microsoft.GSL::GSL
  PRIVATE fmt::fmt)
target_include_directories(common INTERFACE "${CMAKE_CURRENT_LIST_DIR}")

# common library unit tests
add_unit_test(common-letter_set.unit-test letter_set.unit-test.cpp wordle)
