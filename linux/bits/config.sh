#!/bin/bash

# run in fresh build directory to prepare Conan and CMake;
# requires that PROJECT_DIR is set to project directory

set -e

PROJECT_DIR=$(cd $(dirname "$0")/../..; pwd)

cmake \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  "$@" \
  "${PROJECT_DIR}"
