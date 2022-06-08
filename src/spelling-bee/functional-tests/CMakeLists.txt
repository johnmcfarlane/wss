add_functional_tests(
  NAME spelling-bee-help
  COMMAND spelling-bee-main --help
  EXPECTED_OUTPUT
    "^Spelling Bee cheater
\\(C\\)2022 John McFarlane

USAGE:\n"
)

add_functional_tests(
  NAME spelling-bee-happy
  COMMAND spelling-bee-main tuvwxyz
  EXPECTED_OUTPUT "^TUT\nTUTTY\nTUTU\nTUX\nUT\nYUTZ\n$"
)

add_functional_tests(
  NAME spelling-bee-bad-command-line-parameter
  COMMAND spelling-bee-main a b
  EXIT_FAILURE
  EXPECTED_OUTPUT "command line error: Unrecognized token: b"
)

add_functional_tests(
  NAME spelling-bee-too-few-letters
  COMMAND spelling-bee-main abcdef
  EXIT_FAILURE
  EXPECTED_OUTPUT "^6 letters provided; 7 letters expected\n$"
)

add_functional_tests(
  NAME spelling-bee-bad-letter
  COMMAND spelling-bee-main abcdef7
  EXIT_FAILURE
  EXPECTED_OUTPUT "^'7', isn't a letter\n$"
)

add_functional_tests(
  NAME spelling-bee-too-few-unique-letters
  COMMAND spelling-bee-main abcdeff
  EXIT_FAILURE
  EXPECTED_OUTPUT "^6 unique letters provided; 7 unique letters expected\n$"
)
