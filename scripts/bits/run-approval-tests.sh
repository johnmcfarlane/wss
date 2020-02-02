#!/bin/bash

# run after build.sh to test output of approval tests

set -eo pipefail

BITS_DIR=$(cd "$(dirname "$0")"; pwd)
TESTS_PATH=$1

"${BITS_DIR}/invoke-approval-tests.sh" \
  "${BITS_DIR}/run-approval-test.sh" "${TESTS_PATH}"
