add_functional_tests(
  NAME wordle
  COMMAND wordle-main tales00110,clone02202,globe02212
  EXPECTED_OUTPUT "^BLOKE\n$"
)

add_functional_tests(
  NAME wordle-bad-letter
  COMMAND wordle-main abcd_00000
  EXIT_FAILURE
  EXPECTED_OUTPUT "^guess, \"abcd_\" contains invalid letter, '_'\n$"
)

add_functional_tests(
  NAME wordle-bad-move
  COMMAND wordle-main abcde0000
  EXIT_FAILURE
  EXPECTED_OUTPUT
    "^Move, \"abcde0000\", isn't in correct format: 5 letters and 5 numbers.\n$"
)

add_functional_tests(
  NAME wordle-bad-score1
  COMMAND wordle-main abcde00300
  EXIT_FAILURE
  EXPECTED_OUTPUT
    "^guess, \"00300\" contains invalid score, 3, at position, 2; \
range is [0..2]\n$"
)

add_functional_tests(
  NAME wordle-bad-score2
  COMMAND wordle-main abcde00!00
  EXIT_FAILURE
  EXPECTED_OUTPUT "^guess, "00!00" contains invalid score, !, at position, 2; \
range is [0..2]\n$"
)

add_functional_tests(
  NAME wordle-bad-score3
  COMMAND wordle-main abcde00300,abcde00000
  EXIT_FAILURE
  EXPECTED_OUTPUT
    "^guess, \"00300\" contains invalid score, 3, at position, 2; \
range is [0..2]\n$"
)

add_functional_tests(
  NAME wordle-cli-error
  COMMAND wordle-main a b
  EXIT_FAILURE
  EXPECTED_OUTPUT "^command line error: Unrecognized token: b\n$"
)

add_functional_tests(
  NAME wordle-contradiction-hit-hit
  COMMAND wordle-main Aaaaa20000,Baaaa20000
  EXIT_FAILURE
  EXPECTED_OUTPUT "^letter 1 of play, \"Baaaa\", is not allowed to be B!\n$"
)

add_functional_tests(
  NAME wordle-contradiction-hit-miss
  COMMAND wordle-main Aaaaa20000,Aaaaa00000
  EXIT_FAILURE
  EXPECTED_OUTPUT "^letter 1 of play, \"Aaaaa\", cannot not be A!\n$"
)

add_functional_tests(
  NAME wordle-contradiction-hit-near
  COMMAND wordle-main Aaaaa20000,Aaaaa10000
  EXIT_FAILURE
  EXPECTED_OUTPUT "^letter 1 of play, \"Aaaaa\", is not allowed to not be A!\n$"
)

add_functional_tests(
  NAME wordle-contradiction-miss-near
  COMMAND wordle-main Aaaaa12222,Aaaaa02222
  EXIT_FAILURE
  EXPECTED_OUTPUT
    "^guess, \"Aaaaa\", implies there are no more than 4 occurrences of A, \
but previous guesses imply there are at least 5\n$"
)

add_functional_tests(
  NAME wordle-contradiction-near-miss
  COMMAND wordle-main Aaaaa02222,Aaaaa12222
  EXIT_FAILURE
  EXPECTED_OUTPUT
    "^guess, \"Aaaaa\", implies there are at least 5 occurrences of A, \
but previous guesses imply there are no more than 4\n$"
)

add_functional_tests(
  NAME wordle-debug
  COMMAND wordle-main --debug
  EXPECTED_OUTPUT "^{.limits={}, .disallowed=\\("","","","","")}\n$"
)

add_functional_tests(
  NAME wordle-debug-move
  COMMAND wordle-main --debug abcde01210
  EXPECTED_OUTPUT
    "^{.limits={A<=0,1<=B,1<=C,1<=D,E<=0}, .disallowed=\\("A","B",\
"ABDEFGHIJKLMNOPQRSTUVWXYZ","D","E")}\n$"
)

add_functional_tests(
  NAME wordle-empty
  COMMAND wordle-main
  EXPECTED_OUTPUT
    "^AAHED
AALII
AARGH
\[^ \]+
ZOWIE
ZUZIM
ZYMES\n$"
)

add_functional_tests(
  NAME wordle-help
  COMMAND wordle-main --help
  EXPECTED_OUTPUT
    "^Wordle cheater
\\(C\\)2022 John McFarlane

USAGE:
  wordle \\[-?|-h|--help] \\[-d|--debug] \\[<history>]

Display usage information.

OPTIONS, ARGUMENTS:
  -?, -h, --help
  -d, --debug             display internal search data for command
  <history>               comma-separated list of 10-character moves, e.g. \
answer is OTHER, you guess TALES then COMET: "tales10020,comet01021"
\n$"
)

add_functional_tests(
  NAME wordle-hit-miss
  COMMAND wordle-main Aaaaa20000,Baaaa00000
  EXPECTED_OUTPUT "^$"
)

add_functional_tests(
  NAME wordle-hit-near
  COMMAND wordle-main Aaaaa20000,Baaaa10000
  EXPECTED_OUTPUT "^$"
)

add_functional_tests(
  NAME wordle-letters-exhausted1
  COMMAND
    wordle-main
    "aaaaa10000,baaaa10000,caaaa10000,daaaa10000,eaaaa10000,faaaa10000,\
gaaaa10000,haaaa10000,iaaaa10000,jaaaa10000,kaaaa10000,laaaa10000,maaaa10000,\
naaaa10000,oaaaa10000,paaaa10000,qaaaa10000,raaaa10000,saaaa10000,taaaa10000,\
uaaaa10000,vaaaa10000,waaaa10000,xaaaa10000,yaaaa10000,zaaaa10000"
  EXIT_FAILURE
  EXPECTED_OUTPUT "^letter 1 of play, \"zaaaa\", is not allowed to not be Z!\n$"
)

add_functional_tests(
  NAME wordle-letters-exhausted2
  COMMAND wordle-main ABBBB10000,ABBBB20000
  EXIT_FAILURE
  EXPECTED_OUTPUT "^letter 1 of play, \"ABBBB\", already cannot be A!\n$"
)

add_functional_tests(
  NAME wordle-zero-results
  COMMAND wordle-main ZZZZZ22222
  EXPECTED_OUTPUT "^$"
)
