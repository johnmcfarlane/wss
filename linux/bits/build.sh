#!/bin/bash

# run in fresh build directory to prepare Conan and CMake;
# requires that PROJECT_DIR is set to project directory

set -e

NUM_CPUS=$(nproc)

cmake \
  --build . \
  "$@" \
    -- \
    --jobs "${NUM_CPUS}"
