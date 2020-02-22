#!/bin/bash

# run in build directory to invoke Clang-Tidy

set -eo pipefail

BITS_DIR=$(cd "$(dirname "$0")"/bits; pwd)

"${BITS_DIR}/init.sh"

"${BITS_DIR}/config.sh" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_CLANG_TIDY=clang-tidy \
  -DCMAKE_CXX_FLAGS_RELEASE="-DNDEBUG"

"${BITS_DIR}/build.sh"

echo success
