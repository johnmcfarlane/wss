#!/bin/bash

# run in build directory to perform dynamic analysis

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)
BITS_DIR=${PROJECT_DIR}/scripts/bits

"${BITS_DIR}/init-conan.sh"

conan profile update settings.compiler=clang conan-profile
conan profile update settings.compiler.version=10 conan-profile
conan profile update settings.compiler.libcxx=libc++ conan-profile

"${BITS_DIR}/build-dependencies.sh"

"${BITS_DIR}/config.sh" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE=" \
    -DGSL_UNENFORCED_ON_CONTRACT_VIOLATION \
    -fno-sanitize-recover=all \
    -fno-omit-frame-pointer \
    -fsanitize=memory \
    -fsanitize-memory-track-origins \
    -stdlib=libc++ \
    -g"

"${BITS_DIR}/build.sh"

"${BITS_DIR}/run-approval-tests.sh" \
  "${PROJECT_DIR}/test/bin"
