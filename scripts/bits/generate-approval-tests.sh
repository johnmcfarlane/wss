#!/bin/bash

# run after build.sh to generate expected output of approval tests

set -eo pipefail

BITS_DIR=$(cd "$(dirname "$0")"; pwd)
TESTS_PATH=$1

"${BITS_DIR}/invoke-approval-tests.sh" \
  "${BITS_DIR}/generate-approval-test.sh" "${TESTS_PATH}"

echo success
