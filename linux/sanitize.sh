#!/bin/bash

# run in build directory to perform dynamic analysis

set -e

BITS_DIR=$(cd $(dirname "$0")/bits; pwd)
LSAN_OPTIONS=verbosity=1:log_threads=1

"${BITS_DIR}/init.sh"

"${BITS_DIR}/config.sh" \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fsanitize-undefined-trap-on-error -DGSL_UNENFORCED_ON_CONTRACT_VIOLATION"

"${BITS_DIR}/build.sh"

"${BITS_DIR}/run-acceptance-tests.sh"
