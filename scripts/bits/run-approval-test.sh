#!/bin/bash

# run after build.sh to test output of a single approval test

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/../..; pwd)
TEST=$1

echo testing "$TEST"...
TEST_DIR="$(dirname "$TEST")"
"${TEST}" "${PROJECT_DIR}" | \
   diff "${TEST_DIR}/expected.stdout" - 
