#!/bin/bash

# run in build directory to perform dynamic analysis

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)
BITS_DIR=${PROJECT_DIR}/scripts/bits
export LSAN_OPTIONS=verbosity=1:log_threads=1
export ASAN_OPTIONS=verify_asan_link_order=0

"${BITS_DIR}/init.sh"

"${BITS_DIR}/config.sh" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-g -fsanitize=address,undefined -fsanitize-undefined-trap-on-error -DNDEBUG"

"${BITS_DIR}/build.sh"

"${BITS_DIR}/sanitize-approval-tests.sh" \
  "${PROJECT_DIR}/test/bin"
