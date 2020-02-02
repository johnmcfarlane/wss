#!/bin/bash

# run after build.sh to test heap usage against a single approval test

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/../..; pwd)
TEST=$1

echo testing "$TEST"...
"${TEST}" "${PROJECT_DIR}" valgrind --tool=memcheck --leak-check=full --error-exitcode=1
