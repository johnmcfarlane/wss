#!/bin/bash

# Run in build directory to perform static analysis using LLVM.

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)

"${PROJECT_DIR}/scripts/bits/init.sh"

scan-build \
  -o /tmp/scan-build \
  --status-bugs \
    cmake \
      "${PROJECT_DIR}" \
      -DCMAKE_CXX_FLAGS="-DNDEBUG"

scan-build \
  -o scan-build \
  --status-bugs \
    "${PROJECT_DIR}/scripts/bits/build.sh" \
      --clean-first

echo success
