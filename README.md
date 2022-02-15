# WSS - Wordscapes Solver

[![test](https://github.com/johnmcfarlane/wss/actions/workflows/test.yml/badge.svg)](https://github.com/johnmcfarlane/wss/actions/workflows/test.yml)

## Introduction

Wordscapes Solver is a collection of word puzzle solvers written in C++.
It serves two purposes:

1. To help users cheat at word games such as
   * [Wordscapes](https://www.peoplefun.com/games),
   * [Words With Friends](https://www.zynga.com/games/words-with-friends-2/) and
   * [Scrabble](https://scrabble.hasbro.com).

1. To provide the template for a modern C++ project.

## Versatility Through Simplicity

WSS makes effective use of CMake and Conan by keeping configuration scripts
minimal, declarative and decoupled. In this way, the code can be configured,
built and tested against the widest variety of compilers, analysers and other
development tools.

WSS consists of multiple CMake targets and thousands of lines of code.
Yet, the essential configuration files amounts to a 15-line conanfile and
only 97 lines of CMake. This is possible because the build system is separated
from other aspects of project management, such as toolchain configuration and
dependency management. It does one thing well: describing binaries.

## Continuous Integration

The project's CI pipeline demonstrates how to maintain high-quality C++ by
running automated tests against tools such as:

* [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html),
* [CppCheck](http://cppcheck.net/) static analyser,
* [Clang](https://clang.llvm.org/) and [GCC](https://gcc.gnu.org/) compilers,
* [Clang Static Analyzer](https://clang-analyzer.llvm.org/),
* [Clang-Tidy](https://clang.llvm.org/extra/clang-tidy/) C++ linter and static
  analyser,
* [pre-commit](https://pre-commit.com/) linting framework with
  formatting and correctness checks for:
  * Bash
  * C++
  * CMake
  * JSON
  * Markdown
  * Python
  * YAML
* [UndefinedBehaviorSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html).
* [Valgrind](https://valgrind.org).

Developers are invited to add their favourite tools, or to use this project as
the starting point for their own C++ projects.

## Instructions

WSS is built and tested on Linux.
It is designed to be easy to build and to run with:

* Conan package manager,
* CMake build system generator,
* A C++20-compatible GCC or Clang compiler.

To generate an example word list using the _rack_ program

1. create an empty build directory,

   ```sh
   mkdir -p build
   cd build/
   ```

1. install package dependencies,

   ```sh
   conan install --build=missing <path-to-wss>
   ```

1. then configure, build, test and install the programs:

   ```sh
   conan build <path-to-wss>
   ```

1. You can now run the programs, e.g.

   ```sh
   package/bin/rack abc
   ```

   ...which prints a list of words from the given letters:

   > ABC  
   > BAC  
   > CAB  
   > AB  
   > BA

### Troubleshooting

Conan maintains backward ABI compatibility with GCC v5.
Pay attention to warnings emitted by `conan` while initialising your environment.
And if you see errors that mention `abi:cxx11` such as...

> main.cpp:(.text+0x53a2): undefined reference to
  `fmt::v8::vformat[abi:cxx11](fmt::v8::basic_string_view<char>,
  fmt::v8::basic_format_args<fmt::v8::basic_format_context<fmt::v8::appender,
  char> >)'

...then try the following command:

```sh
conan profile update settings.compiler.libcxx=libstdc++11 default
```
