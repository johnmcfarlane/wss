#!/bin/bash

# run in build directory to test output of acceptance tests

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)
NUM_CPUS=$(nproc)

cmake --build . -- --jobs ${NUM_CPUS}

find "${PROJECT_DIR}/test/bin" -name "test.sh" | while read TEST; do
  echo testing $TEST...
  TEST_DIR="$(dirname $TEST)"
  "${TEST}" "${PROJECT_DIR}" | \
     diff "${TEST_DIR}/expected.stdout" - 
  echo ... success
done;

echo Tests passed
