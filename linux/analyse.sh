#!/bin/bash

# run in build directory to perform static analysis

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)
NUM_CPUS=$(nproc)

"${PROJECT_DIR}/linux/init-build-dir.sh"
cmake \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  "${PROJECT_DIR}"
cmake --build . -- --jobs ${NUM_CPUS}

run-clang-tidy -p . -j ${NUM_CPUS}

echo success
