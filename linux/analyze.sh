#!/bin/bash

# run in build directory to perform static analysis

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)
NUM_CPUS=$(nproc)

"${PROJECT_DIR}/linux/init-build-dir.sh"

scan-build cmake "${PROJECT_DIR}"

scan-build cmake --build . -- --jobs ${NUM_CPUS}

echo success
