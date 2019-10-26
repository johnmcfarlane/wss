#!/usr/bin/env bash

set -e

PROJECT_DIR="$(cd $(dirname $0)/.. && pwd )"
NUM_CPUS=$(nproc)

conan install --build=missing "${PROJECT_DIR}"
cmake -DCMAKE_CXX_FLAGS="-fsanitize=undefined -fsanitize-undefined-trap-on-error" "${PROJECT_DIR}"
cmake --build . -- -j ${NUM_CPUS}
./scrabble abcdefghijklmnopqrstuvwxyz? -n 8 > /dev/null

echo success
