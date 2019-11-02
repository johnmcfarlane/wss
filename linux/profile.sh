#!/bin/bash

# run in build directory to perform dynamic analysis

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)
NUM_CPUS=$(nproc)

"${PROJECT_DIR}/linux/config-cmake.sh" \
  -DCMAKE_CXX_FLAGS="-DCMAKE_BUILD_TYPE=RelWithDebInfo -g -DGSL_UNENFORCED_ON_CONTRACT_VIOLATION -fno-omit-frame-pointer"

cmake --build . -- --jobs ${NUM_CPUS}

perf record -g ./src/play/play abcdefghijklmnopqrstuvwxyz_abcdefghijklmnopqrstuvwxyz_ ../test/bin/play/game14/9/tiles.csv
perf report -g 'graph,0.5,caller'
