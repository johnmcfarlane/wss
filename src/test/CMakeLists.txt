find_package(Catch2 REQUIRED)
find_package(fmt REQUIRED)

# helper to add unit test
function(add_unit_test name source_file)
  add_executable(${name} ${source_file})

  find_package(Catch2 REQUIRED)
  target_link_libraries(${name} PRIVATE ${ARGN} Catch2::Catch2 common unit-test)

  add_test(${name} ${name})
  set_tests_properties(${name} PROPERTIES LABELS unit)
endfunction()

add_library(unit-test unit-test.cpp)
target_link_libraries(unit-test PRIVATE Catch2::Catch2 common)

# helper to add functional tests
function(add_functional_tests)
  set(options EXIT_FAILURE)
  set(oneValueArgs NAME EXPECTED_OUTPUT)
  set(multiValueArgs COMMAND EXPECTED_FILES)
  cmake_parse_arguments(
    PARAMS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN}
  )

  # exit test: test that program exits with expected status code
  set(exit_test_name ${PARAMS_NAME}.exit-test)
  add_test(NAME ${exit_test_name} COMMAND ${PARAMS_COMMAND})
  set_tests_properties(
    ${exit_test_name} PROPERTIES LABELS functional WILL_FAIL
                                 ${PARAMS_EXIT_FAILURE}
  )

  # output test: test that program emits expected stdout / stderr
  set(output_test_name ${PARAMS_NAME}.output-test)
  add_test(NAME ${output_test_name} COMMAND ${PARAMS_COMMAND})
  set_tests_properties(
    ${output_test_name}
    PROPERTIES DEPENDS ${exit_test_name} PASS_REGULAR_EXPRESSION
               ${PARAMS_EXPECTED_OUTPUT}
  )

  foreach(output_file ${PARAMS_EXPECTED_FILES})
    # file test: test that program writes expected file
    set(output_file_test_name ${PARAMS_NAME}.diff.${output_file})
    add_test(NAME ${output_file_test_name}
             COMMAND ${CMAKE_COMMAND} -E compare_files
                     ${CMAKE_CURRENT_LIST_DIR}/${output_file} ${output_file}
    )
    set_tests_properties(
      ${output_file_test_name} PROPERTIES DEPENDS ${output_test_name}
    )
  endforeach()
endfunction()

# helper to add fuzz test
function(add_fuzz_test name source_file)
  add_executable(${name} ${source_file})
  target_compile_options(${name} PRIVATE ${WSS_FUZZER_FLAGS})
  target_link_libraries(
    ${name} PRIVATE ${ARGN} common fuzz-test ${WSS_FUZZER_LINKER_FLAGS}
  )
  add_dependencies(fuzz-tests ${name})

  add_test(${name} ${name} ${WSS_FUZZER_TEST_PARAMETERS})
endfunction()

add_library(fuzz-test fuzz-test.cpp)
target_include_directories(fuzz-test PUBLIC .)
target_compile_options(fuzz-test PUBLIC ${WSS_FUZZER_FLAGS})
target_link_libraries(
  fuzz-test
  PRIVATE common ${WSS_FUZZER_LINKER_FLAGS}
  PUBLIC fmt::fmt
)

add_custom_target(fuzz-tests COMMENT "building fuzz tests")
