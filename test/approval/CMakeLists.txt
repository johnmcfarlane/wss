# Create a test which executes the test-failure or test-success command
function(generate_approval_test approval_test_command will_fail)
  get_filename_component(approval_test_directory ${approval_test_command}
                         DIRECTORY)
  file(RELATIVE_PATH test_name ${CMAKE_SOURCE_DIR} ${approval_test_command})
  add_test(
    NAME ${test_name}
    COMMAND ${approval_test_command}
    WORKING_DIRECTORY ${approval_test_directory})
  set_tests_properties(
    ${test_name}
    PROPERTIES
      ENVIRONMENT
      "\
PATH=$<TARGET_FILE_DIR:generate>:\
$<TARGET_FILE_DIR:play>:\
$<TARGET_FILE_DIR:rack>:\
$<TARGET_FILE_DIR:wordle>"
      WILL_FAIL
      ${will_fail})
endfunction()

# Create a test which executes the test-failure or test-success command
function(generate_approval_tests globbing_expression will_fail)
  file(
    GLOB_RECURSE approval_test_commands
    LIST_DIRECTORIES false
    ${globbing_expression})
  foreach(approval_test_command ${approval_test_commands})
    generate_approval_test(${approval_test_command} ${will_fail})
  endforeach()
endfunction()

# Turn all commands called "test-success" into passing tests.
generate_approval_tests("test-success" OFF)

# Turn all commands called "test-failure" into failing tests.
generate_approval_tests("test-failure" ON)
