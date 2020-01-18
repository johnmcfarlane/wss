#!/bin/bash

# run in build directory to test output of approval tests

set -e

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)

"${PROJECT_DIR}/linux/build.sh"

find "${PROJECT_DIR}/test/bin" -name "test.sh" | while read -r TEST; do
  echo testing "$TEST"...
  TEST_DIR="$(dirname "$TEST")"
  "${TEST}" "${PROJECT_DIR}" | \
     diff "${TEST_DIR}/expected.stdout" - 
  echo ... success
done;

echo Tests passed
