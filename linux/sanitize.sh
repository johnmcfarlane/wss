#!/bin/bash

# run in build directory to perform dynamic analysis

set -eo pipefail

BITS_DIR=$(cd "$(dirname "$0")"/bits; pwd)
export LSAN_OPTIONS=verbosity=1:log_threads=1

"${BITS_DIR}/init.sh"

"${BITS_DIR}/config.sh" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-g -fsanitize=address,undefined -fsanitize-undefined-trap-on-error -DGSL_UNENFORCED_ON_CONTRACT_VIOLATION"

"${BITS_DIR}/build.sh"

"${BITS_DIR}/run-approval-tests.sh"
