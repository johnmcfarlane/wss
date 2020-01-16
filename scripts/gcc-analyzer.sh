#!/bin/bash

# Run in build directory to perform static analysis using GCC.

set -eu

BITS_DIR=$(cd "$(dirname "$0")"/bits; pwd)
NUM_CPUS=$(nproc)

"${BITS_DIR}/init.sh"

"${BITS_DIR}/config.sh" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-fanalyzer -DGSL_UNENFORCED_ON_CONTRACT_VIOLATION"

"${BITS_DIR}/build.sh" --target lexicon

run-clang-tidy -p . -j "${NUM_CPUS}"

echo success
