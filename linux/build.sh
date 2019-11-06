#!/bin/bash

# run in fresh build directory to prepare Conan and CMake;
# requires that PROJECT_DIR is set to project directory

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)

"${PROJECT_DIR}/linux/bits/init.sh"

"${PROJECT_DIR}/linux/bits/config.sh" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-DGSL_UNENFORCED_ON_CONTRACT_VIOLATION"

"${PROJECT_DIR}/linux/bits/build.sh"
