SET(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic")

# scrabble_lexicon.cpp and wwf_lexicon.cpp are enormous data definition sources.
# They take an exceedingly long time to compile under GCC if the optimizer is
# enabled.
SET(WSS_LEXICON_COMPILE_OPTIONS "-O0")
