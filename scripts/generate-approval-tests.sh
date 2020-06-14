#!/bin/bash

# run in build directory to reset expected output of approval tests

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)

"${PROJECT_DIR}/scripts/build.sh" \
  -DCMAKE_BUILD_TYPE=Release

"${PROJECT_DIR}/scripts/bits/generate-approval-tests.sh" \
  "${PROJECT_DIR}/test/bin"
