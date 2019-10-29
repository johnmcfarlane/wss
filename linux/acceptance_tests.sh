#!/bin/bash

# run in build directory to perform dynamic analysis

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)
NUM_CPUS=$(nproc)

cmake --build . -- --jobs ${NUM_CPUS}

run_acceptance_test() {
  echo Testing $1
  TEST_DIR="${PROJECT_DIR}/test/bin/$1"
  "${TEST_DIR}/test.sh" "${PROJECT_DIR}" | diff "${TEST_DIR}/expected.stdout" -
  echo ... success
}

run_acceptance_test "play/game1/1"
run_acceptance_test "play/game1/2"

run_acceptance_test "play/game2/4"
run_acceptance_test "play/game2/5"
run_acceptance_test "play/game2/6"
run_acceptance_test "play/game2/7"
run_acceptance_test "play/game2/8"
run_acceptance_test "play/game2/10"
run_acceptance_test "play/game2/12"
run_acceptance_test "play/game2/14"

run_acceptance_test "play/game3/1"
run_acceptance_test "play/game3/3"
run_acceptance_test "play/game3/5"
run_acceptance_test "play/game3/6"
run_acceptance_test "play/game3/7"
run_acceptance_test "play/game3/8"
run_acceptance_test "play/game3/9"

run_acceptance_test "rack/1"

echo Tests passed
