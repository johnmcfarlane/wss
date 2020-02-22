#!/bin/bash

# run in build directory to measure performance using perf

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)

"${PROJECT_DIR}/scripts/bits/init.sh"

"${PROJECT_DIR}/scripts/bits/config.sh" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-g -DNDEBUG -fno-omit-frame-pointer"

"${PROJECT_DIR}/scripts/bits/build.sh"

perf \
  record -g \
  ./src/play/play \
    abcdefghijklmnopqrstuvwxyz_abcdefghijklmnopqrstuvwxyz_ \
    "${PROJECT_DIR}"/test/bin/play/game14/9/tiles.txt
perf report -g 'graph,0.5,caller'
