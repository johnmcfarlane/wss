execute_process(
  COMMAND ${WSS_TEST_COMMAND}
  ERROR_FILE "expected.txt"
  OUTPUT_FILE "expected.txt"
  RESULT_VARIABLE exit-code)
if(${exit-code})
  message(
    FATAL_ERROR
      "script \"${WSS_TEST_COMMAND}\" exited with status ${exit-code}\n")
else()
  message("script \"${WSS_TEST_COMMAND}\" exited with status ${exit-code}\n")
endif()
