#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")"; pwd)
PROJECT_DIR=$1
shift

"$@" ./src/play/play \
  cylooee \
"${SCRIPT_DIR}/tiles.txt" \
  "${PROJECT_DIR}/boards/wwf_regular.txt" 
