# Create a test which executes the test-failure or test-success command
function(generate_approval_test approval_test_command will_fail)
  get_filename_component(approval_test_directory ${approval_test_command}
                         DIRECTORY)
  add_test(
    NAME ${approval_test_command}
    COMMAND ${approval_test_command}
    WORKING_DIRECTORY ${approval_test_directory})
  set_tests_properties(
    ${approval_test_command}
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

# Turn all commands called "test-success" into passing tests.
file(
  GLOB_RECURSE approval_test_success_commands
  LIST_DIRECTORIES false
  "test-success")
foreach(approval_test_command ${approval_test_success_commands})
  generate_approval_test(${approval_test_command} OFF)
endforeach()

# Turn all commands called "test-failure" into failing tests.
file(
  GLOB_RECURSE approval_test_fail_commands
  LIST_DIRECTORIES false
  "test-failure")
foreach(approval_test_command ${approval_test_fail_commands})
  generate_approval_test(${approval_test_command} ON)
endforeach()
