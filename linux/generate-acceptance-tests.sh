#!/bin/bash

# run in build directory to perform dynamic analysis

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)
NUM_CPUS=$(nproc)

cmake --build . -- --jobs ${NUM_CPUS}

find "${PROJECT_DIR}/test/bin" -name "test.sh" | while read TEST; do
  TEST_DIR="$(dirname $TEST)"
  "${TEST}" "${PROJECT_DIR}" > "${TEST_DIR}/expected.stdout" 
done;
