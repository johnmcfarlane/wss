#!/bin/bash

# run in fresh build directory to prepare Conan and CMake;
# requires that PROJECT_DIR is set to project directory

set -e

PROJECT_DIR=$(cd $(dirname "$0")/../..; pwd)
NUM_CPUS=$(nproc)

cmake \
  --build . \
  "$@" \
    -- \
    --jobs ${NUM_CPUS}
