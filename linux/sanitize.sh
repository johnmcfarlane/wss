#!/bin/bash

# run in build directory to perform dynamic analysis

set -ex

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)

"${PROJECT_DIR}/linux/config-cmake.sh" \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fsanitize-undefined-trap-on-error -DGSL_UNENFORCED_ON_CONTRACT_VIOLATION"

LSAN_OPTIONS=verbosity=1:log_threads=1

"${PROJECT_DIR}/linux/test.sh"
