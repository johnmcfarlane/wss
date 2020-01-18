#!/bin/bash

# run in build directory to measure test code coverage

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/.. || exit; pwd)

"${PROJECT_DIR}/linux/bits/init.sh"

"${PROJECT_DIR}/linux/bits/config.sh" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-coverage -fno-exceptions"

"${PROJECT_DIR}/linux/bits/build.sh"

"${PROJECT_DIR}/linux/bits/run-approval-tests.sh"

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
