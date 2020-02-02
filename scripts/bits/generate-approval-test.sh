#!/bin/bash

# run after build.sh to generate expected output of approval tests

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/../..; pwd)
TEST=$1

TEST_DIR="$(dirname "$TEST")"
"${TEST}" "${PROJECT_DIR}" > \
  "${TEST_DIR}/expected.stdout" 
