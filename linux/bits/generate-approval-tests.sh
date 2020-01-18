#!/bin/bash

# run in build directory to reset expected output of approval tests

set -e

PROJECT_DIR=$(cd $(dirname "$0")/../..; pwd)

find "$1" -name "test.sh" | while read TEST; do
  TEST_DIR="$(dirname $TEST)"
  "${TEST}" "${PROJECT_DIR}" > "${TEST_DIR}/expected.stdout" 
done;
