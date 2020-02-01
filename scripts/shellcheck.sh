#!/bin/bash

# run in build directory to invoke Clang-Tidy

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)

find "${PROJECT_DIR}" -name "*.sh" | while read -r TEST; do
  shellcheck \
    --check-sourced \
    --color=always \
    --external-sources \
    --severity=info \
    --shell=bash \
    "$TEST"
done;

echo success
