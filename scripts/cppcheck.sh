#!/bin/bash

# run in build directory to invoke Clang-Tidy

set -eo pipefail

BITS_DIR=$(cd "$(dirname "$0")"/bits; pwd)
NUM_CPUS=$(nproc)

"${BITS_DIR}/init.sh"

"${BITS_DIR}/config.sh" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

set +e

cppcheck \
  --enable=all \
  --error-exitcode=1 \
  --inline-suppr \
  -j "${NUM_CPUS}" \
  --project=compile_commands.json

if [[ $? -ne 0 ]] ; then
  echo failure
  exit 1
else
  echo success
  exit 0
fi
