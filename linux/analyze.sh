#!/bin/bash

# run in build directory to perform static analysis

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)
NUM_CPUS=$(nproc)

"${PROJECT_DIR}/linux/init-build-dir.sh"

scan-build \
  -o /tmp/scan-build \
  --status-bugs \
    cmake \
      "${PROJECT_DIR}" \
      -DCMAKE_CXX_FLAGS="-DGSL_UNENFORCED_ON_CONTRACT_VIOLATION"

scan-build \
  -o /tmp/scan-build \
  --status-bugs \
    cmake \
      --build . \
        -- \
        --jobs ${NUM_CPUS}

echo success
