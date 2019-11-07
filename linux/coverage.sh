#!/bin/bash

# run in build directory to measure test code coverage

set -e

PROJECT_DIR=$(cd $(dirname "$0")/..; pwd)

"${PROJECT_DIR}/linux/bits/init.sh"

"${PROJECT_DIR}/linux/bits/config.sh" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-coverage"

"${PROJECT_DIR}/linux/bits/build.sh"

"${PROJECT_DIR}/linux/bits/run-acceptance-tests.sh"

lcov \
  --capture \
  --base-directory ${PROJECT_DIR} \
  --directory . \
  --gcov gcov-9 \
  --include "${PROJECT_DIR}/*" \
  --rc lcov_branch_coverage=1 \
  --output-file coverage-info

genhtml \
  coverage-info \
  --branch-coverage \
  --output-directory /tmp/coverage-report | tee coverage-log

grep "functions..: 100.0%" coverage-log > /dev/null || echo missing function coverage
grep "lines......: 100.0%" coverage-log > /dev/null || echo missing line coverage

echo '
success'
