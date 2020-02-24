#!/bin/bash

# run in build directory to perform Valgrind memory checks

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)
BITS_DIR=${PROJECT_DIR}/scripts/bits

"${BITS_DIR}/init.sh"

"${BITS_DIR}/config.sh" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-g"

"${BITS_DIR}/build.sh"

set +e

"${BITS_DIR}/valgrind-approval-tests.sh" \
  "${PROJECT_DIR}/test/bin"

if [[ $? -ne 0 ]] ; then
  echo failure
  exit 1
else
  echo success
  exit 0
fi
