#!/bin/bash

# run in build directory to reset expected output of acceptance tests

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)

"${PROJECT_DIR}/linux/build.sh"

"${PROJECT_DIR}/linux/bits/generate-acceptance-tests.sh"

echo success
