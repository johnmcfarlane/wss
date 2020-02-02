#!/bin/bash

# runs given script on approval tests found in the given path

set -eo pipefail

NUM_CPUS=$(nproc)
SCRIPT=$1
TESTS_PATH=$2

find "${TESTS_PATH}" -name "test.sh" | \
  parallel \
    -j "${NUM_CPUS}" \
    "${SCRIPT}"
