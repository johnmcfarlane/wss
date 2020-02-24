#!/bin/bash

# run in build directory to invoke Clang-Tidy

set -eo pipefail

BITS_DIR=$(cd "$(dirname "$0")"/bits; pwd)

"${BITS_DIR}/init.sh"

"${BITS_DIR}/config.sh" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_CLANG_TIDY=clang-tidy \
  -DCMAKE_CXX_FLAGS_RELEASE="-DNDEBUG"

set +e

"${BITS_DIR}/build.sh"

if [[ $? -ne 0 ]] ; then
  echo failure
  exit 1
else
  echo success
  exit 0
fi
