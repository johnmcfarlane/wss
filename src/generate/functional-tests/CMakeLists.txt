add_functional_tests(
  NAME generate-happy
  COMMAND generate ${CMAKE_CURRENT_LIST_DIR}/happy-lexicon.txt lexicon
          happy-source
  EXPECTED_OUTPUT "^loading\\.\\.\\.\ncompressing\\.\\.\\.\nsaving\\.\\.\\.\n\$"
  EXPECTED_FILES happy-source.h happy-source.cpp
)

add_functional_tests(
  NAME generate-cli-help
  COMMAND generate --help
  EXPECTED_OUTPUT
    "^wss lexicon source file generator
\\(C\\)2019 John McFarlane

USAGE:
  generate \\[-?|-h|--help] \\[<lexicon>] \\[<name>] \\[<source>]

Display usage information.

OPTIONS, ARGUMENTS:
  -?, -h, --help
  <lexicon>               text file containing list of words
  <name>                  Name of lexicon
  <source>                source filename\n\$"
)

add_functional_tests(
  NAME generate-cli-parse-error
  COMMAND generate a b c d e
  EXIT_FAILURE
  EXPECTED_OUTPUT "^command line error: Unrecognized token: d\n"
)

add_functional_tests(
  NAME generate-invalid-wordform
  COMMAND generate ${CMAKE_CURRENT_LIST_DIR}/invalid-wordform-lexicon.txt
          lexicon expected_source
  EXIT_FAILURE
  EXPECTED_OUTPUT "^error: invalid character, 36, in wordform '\\$a' in \
${CMAKE_CURRENT_LIST_DIR}/invalid-wordform-lexicon.txt\n"
)

add_functional_tests(
  NAME generate-missing-lexicon-parameter
  COMMAND generate
  EXIT_FAILURE
  EXPECTED_OUTPUT "^command line error: Expected: <lexicon>\n"
)

add_functional_tests(
  NAME generate-missing-name-parameter
  COMMAND generate ${CMAKE_CURRENT_LIST_DIR}/happy-lexicon.txt
  EXIT_FAILURE
  EXPECTED_OUTPUT "^command line error: Expected: <name>\n"
)

add_functional_tests(
  NAME generate-missing-source-parameter
  COMMAND generate ${CMAKE_CURRENT_LIST_DIR}/happy-lexicon.txt lexicon
  EXIT_FAILURE
  EXPECTED_OUTPUT "^command line error: Expected: <source>\n"
)

add_functional_tests(
  NAME generate-missing-lexicon
  COMMAND generate missing_file.txt some-name some-source
  EXIT_FAILURE
  EXPECTED_OUTPUT "^error: Could not open 'missing_file.txt'.\n"
)

add_functional_tests(
  NAME generate-missing-newline
  COMMAND generate ${CMAKE_CURRENT_LIST_DIR}/missing-newline-lexicon.txt
          some-name some-source
  EXIT_FAILURE
  EXPECTED_OUTPUT "^error: missing newline at end of 'a' in \
${CMAKE_CURRENT_LIST_DIR}/missing-newline-lexicon.txt\n"
)

add_functional_tests(
  NAME generate-duplicate
  COMMAND generate ${CMAKE_CURRENT_LIST_DIR}/duplicate-lexicon.txt lexicon
          simple-source
  EXPECTED_OUTPUT "^loading\\.\\.\\.\ncompressing\\.\\.\\.\nsaving\\.\\.\\.\n\$"
  EXPECTED_FILES simple-source.h simple-source.cpp
)

add_functional_tests(
  NAME generate-shuffled
  COMMAND generate ${CMAKE_CURRENT_LIST_DIR}/shuffled-lexicon.txt lexicon
          simple-source
  EXPECTED_OUTPUT "^loading\\.\\.\\.\ncompressing\\.\\.\\.\nsaving\\.\\.\\.\n\$"
  EXPECTED_FILES simple-source.h simple-source.cpp
)
