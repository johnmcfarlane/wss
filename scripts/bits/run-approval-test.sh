#!/bin/bash

# run after build.sh to test output of a single approval test

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/../..; pwd)
TEST=$1
TEST_DIR="$(dirname "$TEST")"
RELATIVE_PATH=$(realpath "$TEST_DIR" --relative-to "$PROJECT_DIR")

echo testing "$RELATIVE_PATH" ...

set +e
"${TEST}" "${PROJECT_DIR}" | \
   diff "${TEST_DIR}/expected.stdout" - 

if [[ $? -ne 0 ]] ; then
  printf "\033[0;31mFAILED\033[0m %s\n" "$RELATIVE_PATH"
  exit 1
fi

printf "\033[0;32mPASSED\033[0m\n"
