#!/bin/bash

# Run to build targets in the working directory.

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/..; pwd)

"${PROJECT_DIR}/scripts/bits/init.sh"

"${PROJECT_DIR}/scripts/bits/config.sh" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-DNDEBUG"

"${PROJECT_DIR}/scripts/bits/build.sh"
