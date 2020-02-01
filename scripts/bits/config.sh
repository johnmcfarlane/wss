#!/bin/bash

# run after init.sh and before build.sh to configure build system

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/../..; pwd)

cmake \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  -DCMAKE_TOOLCHAIN_FILE="${PROJECT_DIR}"/scripts/bits/toolchain.cmake \
  "$@" \
  "${PROJECT_DIR}"
