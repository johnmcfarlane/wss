#!/bin/bash

# runs given script on approval tests found in the given path

set -eo pipefail

NUM_CPUS=$(nproc)
SCRIPT=$1
TESTS_PATH=$2

set +e

find "${TESTS_PATH}" -name "test.sh" | \
  parallel \
    -j "${NUM_CPUS}" \
    "${SCRIPT}"

if [[ $? -ne 0 ]] ; then
  echo failure
  exit 1
else
  echo success
  exit 0
fi
