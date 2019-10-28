#!/bin/bash

# run in build directory to perform dynamic analysis

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)
NUM_CPUS=$(nproc)

cmake --build . -- --jobs ${NUM_CPUS}

"${PROJECT_DIR}/test/src/play/case1/test.sh"
"${PROJECT_DIR}/test/src/rack/case2/test.sh"
"${PROJECT_DIR}/test/src/play/case3/test.sh"
"${PROJECT_DIR}/test/src/play/case4/test.sh"
"${PROJECT_DIR}/test/src/play/case5/test.sh"
"${PROJECT_DIR}/test/src/play/case6/test.sh"

echo Tests passed
