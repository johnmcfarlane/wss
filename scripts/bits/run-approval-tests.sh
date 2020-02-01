#!/bin/bash

# run after build.sh to test output of approval tests

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/../..; pwd)

find "$1" -name "test.sh" | while read -r TEST; do
  echo testing "$TEST"...
  TEST_DIR="$(dirname "$TEST")"
  "${TEST}" "${PROJECT_DIR}" | \
     diff "${TEST_DIR}/expected.stdout" - 
  echo ... success
done;

echo Tests passed
