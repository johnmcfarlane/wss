#!/bin/bash

# run in build directory to perform dynamic analysis

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)
NUM_CPUS=$(nproc)

"${PROJECT_DIR}/linux/init-build-dir.sh"
cmake \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fsanitize-undefined-trap-on-error" \
  "${PROJECT_DIR}"
cmake --build . -- --jobs ${NUM_CPUS}

./scrabble abcdefghijklmnopqrstuvwxyz? --min-length 8 > /dev/null

echo success
