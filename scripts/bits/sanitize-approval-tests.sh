#!/bin/bash

# run after build.sh to test UB & heap usage against approval tests

set -eo pipefail

BITS_DIR=$(cd "$(dirname "$0")"; pwd)
TESTS_PATH=$1

"${BITS_DIR}/invoke-approval-tests.sh" \
  "${BITS_DIR}/sanitize-approval-test.sh" "${TESTS_PATH}"
