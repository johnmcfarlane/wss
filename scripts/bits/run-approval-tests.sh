#!/bin/bash

# run after build.sh to test output of approval tests

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/../..; pwd)
NUM_CPUS=$(nproc)

find "$1" -name "test.sh" | \
  parallel \
    -j "${NUM_CPUS}" \
    "${PROJECT_DIR}"/scripts/bits/run-approval-test.sh

echo Tests passed
