#!/bin/bash

# run in build directory to invoke Clang-Tidy

set -eo pipefail

BITS_DIR=$(cd "$(dirname "$0")"/bits; pwd)
NUM_CPUS=$(nproc)

"${BITS_DIR}/init.sh"

"${BITS_DIR}/config.sh" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cppcheck \
  --enable=all \
  --error-exitcode=1 \
  --inline-suppr \
  -j "${NUM_CPUS}" \
  --project=compile_commands.json

echo success
