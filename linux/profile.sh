#!/bin/bash

# run in build directory to measure performance using perf

set -e

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)

"${PROJECT_DIR}/linux/bits/init.sh"

"${PROJECT_DIR}/linux/bits/config.sh" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-g -DGSL_UNENFORCED_ON_CONTRACT_VIOLATION -fno-omit-frame-pointer"

"${PROJECT_DIR}/linux/bits/build.sh"

perf \
  record -g \
  ./src/play/play \
    abcdefghijklmnopqrstuvwxyz_abcdefghijklmnopqrstuvwxyz_ \
    "${PROJECT_DIR}"/test/bin/play/game14/9/tiles.csv
perf report -g 'graph,0.5,caller'
