#!/bin/bash

# run in build directory to reset expected output of approval tests

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)

"${PROJECT_DIR}/linux/build.sh"

"${PROJECT_DIR}/linux/bits/generate-approval-tests.sh"

echo success
