#!/bin/bash

# run after build.sh to test heap usage against approval tests

set -eo pipefail

BITS_DIR=$(cd "$(dirname "$0")"; pwd)
TESTS_PATH=$1

"${BITS_DIR}/invoke-approval-tests.sh" \
  "${BITS_DIR}/valgrind-approval-test.sh" "${TESTS_PATH}"

echo Valgrind memcheck tests passed
