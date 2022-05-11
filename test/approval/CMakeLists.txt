if(WIN32)
  find_program(PWSH NAMES pwsh powershell REQUIRED)
else()
  include(FindUnixCommands REQUIRED)
endif()

# Create a test which executes the executable with params from the file
function(generate_approval_test executable param_file will_fail)
  get_filename_component(approval_test_directory ${param_file} DIRECTORY)
  file(RELATIVE_PATH test_name ${CMAKE_SOURCE_DIR} ${param_file})
  file(READ ${param_file} parameters)
  string(REPLACE "\n" "" parameters "${parameters}")
  if(WIN32)
    add_test(
      NAME ${test_name}
      COMMAND
        ${PWSH} -Command
        "${executable} ${parameters} | Out-File -Encoding ascii stdout.win"
      WORKING_DIRECTORY ${approval_test_directory}
    )
  elseif(BASH)
    add_test(
      NAME ${test_name}
      COMMAND ${BASH} -c "${executable} ${parameters} > stdout 2> stderr"
      WORKING_DIRECTORY ${approval_test_directory}
    )
  else()
    message(FATAL_ERROR "Unknown shell command for ${CMAKE_HOST_SYSTEM_NAME}")
  endif()
  set_tests_properties(${test_name} PROPERTIES LABELS functional)
  set_tests_properties(${test_name} PROPERTIES WILL_FAIL ${will_fail})
endfunction()

# Create a test which executes the test-failure or test-success command
function(generate_approval_tests executable globbing_expression will_fail)
  file(
    GLOB_RECURSE param_files
    LIST_DIRECTORIES false
    ${globbing_expression}
  )
  foreach(param_file ${param_files})
    generate_approval_test(${executable} ${param_file} ${will_fail})
  endforeach()
endfunction()

# Create tests for the given executable
function(generate_approval_tests_for_exectuable executable)
  # Turn all files called "test-success" into happy tests.
  generate_approval_tests(
    ${executable} "${CMAKE_CURRENT_LIST_DIR}/test-success" OFF
  )

  # Turn all files called "test-failure" into sad tests.
  generate_approval_tests(
    ${executable} "${CMAKE_CURRENT_LIST_DIR}/test-failure" ON
  )
endfunction()

add_subdirectory(generate)
add_subdirectory(wordle)
