#!/usr/bin/env bash

set -e

PROJECT_DIR="$(cd $(dirname $0)/.. && pwd )"
NUM_CPUS=$(nproc)

conan install --build=missing "${PROJECT_DIR}"
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "${PROJECT_DIR}"
cmake --build . -- -j ${NUM_CPUS}
run-clang-tidy -p . -j ${NUM_CPUS}

echo success
