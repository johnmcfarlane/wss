#!/bin/bash

# run in build directory to perform dynamic analysis

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)
NUM_CPUS=$(nproc)

cmake --build . -- --jobs ${NUM_CPUS}

find "${PROJECT_DIR}/test/bin" -name "test.sh" | while read TEST; do
  echo testing $TEST...
  TEST_DIR="$(dirname $TEST)"
  "${TEST}" "${PROJECT_DIR}" > "${TEST_DIR}/expected.stdout" 
  echo ... success
done;

#run_acceptance_test "play/game1/1"
#run_acceptance_test "play/game1/2"
#
#run_acceptance_test "play/game2/4"
#run_acceptance_test "play/game2/5"
#run_acceptance_test "play/game2/6"
#run_acceptance_test "play/game2/7"
#run_acceptance_test "play/game2/8"
#run_acceptance_test "play/game2/10"
#run_acceptance_test "play/game2/12"
#run_acceptance_test "play/game2/14"
#
#run_acceptance_test "play/game3/1"
#run_acceptance_test "play/game3/3"
#run_acceptance_test "play/game3/5"
#run_acceptance_test "play/game3/6"
#run_acceptance_test "play/game3/7"
#run_acceptance_test "play/game3/8"
#run_acceptance_test "play/game3/9"
#
#run_acceptance_test "play/game4/1"
#run_acceptance_test "play/game4/2"
#run_acceptance_test "play/game4/3"
#run_acceptance_test "play/game4/4"
#run_acceptance_test "play/game4/5"
#run_acceptance_test "play/game4/6"
#run_acceptance_test "play/game4/7"
#run_acceptance_test "play/game4/9"
#
#run_acceptance_test "rack/1"

echo Tests passed
