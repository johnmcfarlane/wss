#!/bin/bash

# run after build.sh to test output of a single approval test

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/../..; pwd)
TEST=$1

echo testing "$TEST"...
TEST_DIR="$(dirname "$TEST")"

set +e
stdbuf -o0 "${TEST}" "${PROJECT_DIR}" | \
   diff "${TEST_DIR}/expected.stdout" - 

if [[ $? -ne 0 ]] ; then
  printf "\033[0;31mFAILED\033[0m\n"
  exit 1
fi

printf "\033[0;32mPASSED\033[0m\n"
