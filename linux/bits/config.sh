#!/bin/bash

# run in fresh build directory to prepare Conan and CMake;
# requires that PROJECT_DIR is set to project directory

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/../..; pwd)

cmake \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  -DCMAKE_TOOLCHAIN_FILE="${PROJECT_DIR}"/linux/bits/toolchain.cmake \
  "$@" \
  "${PROJECT_DIR}"
