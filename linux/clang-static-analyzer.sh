#!/bin/bash

# Run in build directory to perform static analysis using LLVM.

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)

"${PROJECT_DIR}/linux/bits/init.sh"

scan-build \
  -o /tmp/scan-build \
  --status-bugs \
    cmake \
      "${PROJECT_DIR}" \
      -DCMAKE_CXX_FLAGS="-DGSL_UNENFORCED_ON_CONTRACT_VIOLATION"

scan-build \
  -o scan-build \
  --status-bugs \
    "${PROJECT_DIR}/linux/bits/build.sh" \
      --clean-first

echo success
