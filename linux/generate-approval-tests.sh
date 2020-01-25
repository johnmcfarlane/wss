#!/bin/bash

# run in build directory to reset expected output of approval tests

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)

"${PROJECT_DIR}/linux/build.sh"

"${PROJECT_DIR}/linux/bits/generate-approval-tests.sh" \
  "${PROJECT_DIR}/test/bin"

echo success