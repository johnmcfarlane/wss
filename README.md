# wss
Wordscapes Solver

## build

```sh
conan remote add -f my-conan-repo https://api.bintray.com/conan/bincrafters/public-conan
conan install --build=missing path/to/wss
cmake path/to/wss
cmake --build .
```

## Invocation

Produces all words of 3 or more letters in descending order of Scrabble score.
Words must contain no more than one of each of *a*, *b*, *c* and any additional
letter.

```sh
./rack --min-length 3 abc?
```

Blanks can be represented by underscure (`_`). They are the same as wildcards
(`?`) except that they have a score of zero.
