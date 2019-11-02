#!/bin/bash

# run in build directory to perform static analysis

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)
NUM_CPUS=$(nproc)

"${PROJECT_DIR}/linux/config-cmake.sh"

cmake --build . --target lexicon -- --jobs ${NUM_CPUS}

run-clang-tidy -p . -j ${NUM_CPUS}

echo success
