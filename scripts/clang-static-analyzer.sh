#!/bin/bash

# Run in build directory to perform static analysis using LLVM.

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)

"${PROJECT_DIR}/scripts/bits/init.sh"

scan-build \
  -o /tmp/scan-build \
  --status-bugs \
  --use-c++=c++ \
  --use-cc=cc \
    cmake \
      "${PROJECT_DIR}" \
      -DCMAKE_CXX_FLAGS="-DNDEBUG"

set +e

scan-build \
  -o scan-build \
  --status-bugs \
  --use-c++=c++ \
  --use-cc=cc \
    "${PROJECT_DIR}/scripts/bits/build.sh" \
      --clean-first

if [[ $? -ne 0 ]] ; then
  echo failure
  exit 1
else
  echo success
  exit 0
fi
