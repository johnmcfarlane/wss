find_package(Catch2 REQUIRED)
find_package(fmt REQUIRED)

add_library(unit-test unit-test.cpp)
target_link_libraries(unit-test PRIVATE Catch2::Catch2 common)

# helper to add unit test
function(add_unit_test name source_file)
  add_executable(${name} ${source_file})

  find_package(Catch2 REQUIRED)
  target_link_libraries(${name} PRIVATE ${ARGN} Catch2::Catch2 common unit-test)

  add_test(${name} ${name})
endfunction()

# helper to add fuzz test
function(add_fuzz_test name source_file)
  add_executable(${name} ${source_file})
  target_compile_options(${name} PRIVATE ${WSS_FUZZER_FLAGS})
  target_link_libraries(${name} PRIVATE ${ARGN} common fuzz-test
                                        ${WSS_FUZZER_LINKER_FLAGS})
  add_dependencies(fuzz-tests ${name})

  add_test(${name} ${name} ${WSS_FUZZER_TEST_PARAMETERS})
endfunction()

add_library(fuzz-test fuzz-test.cpp)
target_include_directories(fuzz-test PUBLIC .)
target_compile_options(fuzz-test PUBLIC ${WSS_FUZZER_FLAGS})
target_link_libraries(
  fuzz-test
  PRIVATE common ${WSS_FUZZER_LINKER_FLAGS}
  PUBLIC fmt::fmt)

add_custom_target(fuzz-tests COMMENT "building fuzz tests")
