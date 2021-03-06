#!/bin/bash

# run in build directory to measure test code coverage

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/.. || exit; pwd)

"${PROJECT_DIR}/scripts/bits/init.sh"

"${PROJECT_DIR}/scripts/bits/config.sh" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-coverage -fno-exceptions -DWSS_DISABLE_ASSERTS"

"${PROJECT_DIR}/scripts/bits/build.sh"

"${PROJECT_DIR}/scripts/bits/run-approval-tests.sh" \
  "${PROJECT_DIR}/test/bin"

lcov \
  --capture \
  --base-directory "${PROJECT_DIR}" \
  --directory . \
  --gcov gcov-9 \
  --include "${PROJECT_DIR}/*" \
  --rc lcov_branch_coverage=1 \
  --output-file coverage-info

genhtml \
  coverage-info \
  --branch-coverage \
  --output-directory coverage-report | tee coverage-log

SUCCESS=1

set +e

grep "functions..: 100.0%" coverage-log
if [[ $? -ne 0 ]] ; then
  echo error: missing function coverage
  SUCCESS=0
fi

grep "lines......: 100.0%" coverage-log
if [[ $? -ne 0 ]] ; then
  echo error: missing line coverage
  SUCCESS=0
fi

grep "branches...: 100.0%" coverage-log
if [[ $? -ne 0 ]] ; then
  echo error: missing branch coverage
  SUCCESS=0
fi

set -eo pipefail

echo report generated in ./coverage-report/

if [[ ${SUCCESS} -eq 0 ]] ; then
  echo failure
  exit 1
else
  echo success
  exit 0
fi
