#!/bin/bash

# run in build directory to test output of approval tests

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)

"${PROJECT_DIR}/scripts/build.sh"

"${PROJECT_DIR}/scripts/bits/run-approval-tests.sh" \
  "${PROJECT_DIR}/test/bin"
