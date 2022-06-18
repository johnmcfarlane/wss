# WSS

[![GitHub](https://github.com/johnmcfarlane/wss/actions/workflows/test.yml/badge.svg)](https://github.com/johnmcfarlane/wss/actions/workflows/test.yml)
[![GitLab](https://gitlab.com/johnmcfarlane/wss/badges/main/pipeline.svg)](https://gitlab.com/johnmcfarlane/wss/-/commits/main)

## Introduction

WSS is a tool written in C++ for suggesting wordles. It serves two purposes:

1. to provide [Wordle](https://www.nytimes.com/games/wordle/) players with a
   list of possible moves, and
1. to provide the template for a maintainable CMake project.

## Requirements

* C++20 compiler
* CMake 3.16

## Word Search

WSS produces a command-line tool, `wordle`, which takes a history of previous
attempts and lists all potentially-winning words.

### Example

After playing the following two moves,

![Image](docs/wordle.png)

you can search for possible WORDLEs:

```sh
wordle TALES20010,TEMPO21001
```

Output:

> THROE  
> TOGUE  
> TONNE  
> TOQUE  
> TORTE  
> TOWIE  
> TRODE  
> TROKE  
> TRONE  
> TROVE  

### Implementation

WSS builds the `wordle` program by consuming a lexicon of 5-letter words,
converting them into C++-encoded data, and then
building them into a program which searches through them for suggestions.

The encoded data takes the form of a Directed Acyclic Word-Graph (DAWG),
similar to the one described in the paper,
[The World's Fastest Scrabble Program](https://www.cs.cmu.edu/afs/cs/academic/class/15451-s06/www/lectures/scrabble.pdf)
(Andrew W. Appel, Guy J. Jacobson, 1988).
It allows algorithms to search through the lexicon in a way that avoids much of
the duplication one typically finds in a word list.
This helps to find solutions in a shorter time.

## Versatility Through Simplicity

WSS also aims to demonstrate a simpler way of organising C++ projects.

Many CMake projects couple tools to the build system explicitly.
This approach is often brittle, inflexible and overly-complex.

WSS makes effective use of CMake by keeping configuration scripts minimal,
declarative and decoupled. In this way, the code can be configured,
built and tested against an open set of toolchains, package managers,
and other development tools.

The codebase represents multiple CMake targets and thousands of lines of code.
Yet, the essential build configuration amounts to just a few hundred lines of
CMake, and everything can be built and tested with just two Conan commands,
or five vcpkg commands. This is possible because the build system is separate
from other aspects of project management, such as toolchain configuration and
dependency management. It does one thing well: describing binaries.

## Continuous Integration

The project's CI pipelines demonstrate how to develop maintainable C++ by
applying the build system to many tools...

Toolchains:

* [Clang](https://clang.llvm.org/)
* [GCC](https://gcc.gnu.org/)
* [MSVC](https://visualstudio.microsoft.com/vs/features/cplusplus/)

Package Managers:

* [Conan](https://conan.io/) Python-based C++ package manager
* [vcpkg](https://vcpkg.io/) CMake-based C++ package manager

Collaboration and CI:

* [GitHub](https://github.com/)+[Actions](https://github.com/features/actions)
  demonstrates [CI](https://github.com/johnmcfarlane/wss/actions) via
  [a GitHub repository](https://github.com/johnmcfarlane/wss)
* [GitLab](https://gitlab.com/) demonstrates [CI](https://gitlab.com/johnmcfarlane/wss/-/pipelines)
  via [a GitLab repository](https://gitlab.com/johnmcfarlane/wss/-/pipelines)

Analysers:

* [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html)
* [CppCheck](http://cppcheck.net/) static analyser
* [Clang Static Analyzer](https://clang-analyzer.llvm.org/)
* [Clang-Tidy](https://clang.llvm.org/extra/clang-tidy/) C++ linter and static
  analyser
* [gcov](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html)+
  [lcov](http://ltp.sourceforge.net/coverage/lcov.php) reporting 100% code coverage
* [graphviz](https://graphviz.org/) representation of package and target dependencies
* [Include what you use](https://include-what-you-use.org/) tool to analyze `#include`s
* [libfuzzer](https://www.llvm.org/docs/LibFuzzer.html) coverage-guided fuzz testing
* [pre-commit](https://pre-commit.com/) linting framework with
  formatting and correctness checks for:
  * Bash
  * C++
  * CMake
  * JSON
  * Markdown
  * Python
  * YAML
* [UndefinedBehaviorSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html)
* [Valgrind](https://valgrind.org)

## Instructions

WSS is built and tested on Linux and Windows.
It is designed to be easy to build and to run with:

* Conan package manager,
* CMake build system generator,
* A C++20-compatible compiler such as Clang, GCC or MSVC.

### Build and Test (Conan)

To build `wordle` using Conan on Linux,

1. create an empty build directory,

   ```sh
   mkdir -p build
   cd build/
   ```

1. install package dependencies,

   ```sh
   conan install --build=missing <path-to-wss>
   ```

1. then configure, build, test, and install the program:

   ```sh
   conan build <path-to-wss>
   ```

1. The program can be found in `package/bin`:

   ```sh
   ./package/bin/wordle
   ```

### Build and Test (vcpkg)

To build `wordle` using vcpkg on Linux,

1. create an empty build directory,

   ```sh
   mkdir -p build
   cd build/
   ```

1. install vcpkg using [these instructions](https://vcpkg.io/en/getting-started.html),

   ```sh
   git clone https://github.com/Microsoft/vcpkg.git
   ./vcpkg/bootstrap-vcpkg.sh
   ```

1. then configure, build, and test the program:

   ```sh
   cmake -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake <path-to-wss>
   cmake --build .
   ctest
   ```

### Use WSS as a Template

You can use WSS as the starting-off point for your own C++ project.
From the [WSS GitHub repository](https://github.com/johnmcfarlane/wss),
click the "Use this template" button in order to
[generate](https://github.com/johnmcfarlane/wss/generate) your own repository.
