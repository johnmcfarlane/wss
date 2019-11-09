#!/bin/bash

# run in build directory to invoke Clang-Tidy

set -e

BITS_DIR=$(cd $(dirname "$0")/bits; pwd)
NUM_CPUS=$(nproc)

"${BITS_DIR}/init.sh"

"${BITS_DIR}/config.sh" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

"${BITS_DIR}/build.sh" --target lexicon

run-clang-tidy -p . -j ${NUM_CPUS}

echo success
