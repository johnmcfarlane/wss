# WSS - Wordscapes Solver

[![CircleCI](https://circleci.com/gh/johnmcfarlane/wss.svg?style=svg)](https://circleci.com/gh/johnmcfarlane/wss)

## Introduction

These utilities are designed to help users cheat at word games such as
[Wordscapes](https://www.peoplefun.com/games), 
[Words With Friends](https://www.zynga.com/games/words-with-friends-2/) and
[Scrabble](http://www.scrabble.com/).

## Instructions

WSS is built and tested on Linux.

To quickly run an example Words With Friends move using input data from the test
suite, run the following commands from the *wss* project directory:

```sh
mkdir -p build
cd build/
conan install --build=missing ..
cmake ..
cmake --build .
src/play/play abcd_ef ../test/bin/play/game1/1/tiles.csv ../boards/wwf_regular.csv
```

Results

>  44  4,4  - JABBED  
>  42  4,4  - JABBeD  
>  40  4,4  - JABBEd  
>  40  4,4  - JABBEr  
>  36  8,15 - BAsED  
>  36  8,15 - CAsED  
>  36  4,4  - JABbED  
> ...

signify:

* word score
* column and row number,
* horizontal (`-`) or vertical (`|`) move 
* word to play, with lower-case letters for blanks

## Continuous Integration

The project is intended to demonstrate how to write safe, modern C++ using free
tools to help prevent bugs. Tools used to help verify the
correctness of the code include:

* [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html),
* [Clang](https://clang.llvm.org/) and [GCC](https://gcc.gnu.org/) compilers,
* [Clang Static Analyzer](https://clang-analyzer.llvm.org/),
* [Clang-Tidy](https://clang.llvm.org/extra/clang-tidy/) C++ linter and static
  analyser,
* [LCOV](http://ltp.sourceforge.net/coverage/lcov.php) /
  [gcov](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html) code coverage tester, and
* [UndefinedBehaviorSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html).

These tools are available via scripts in the *linux* directory and are used as
part of a continuous integration pipeline built from 
[CircleCI](https://circleci.com/gh/johnmcfarlane/wss) and
[GitHub repository](https://github.com/johnmcfarlane/wss). Developers are
encouraged to fork or clone this project as the basis for their own C++
projects. 
