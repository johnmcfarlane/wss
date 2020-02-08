#!/bin/bash

set -eo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")"; pwd)
PROJECT_DIR=$1
shift

"$@" ./src/play/play \
  TSEOZSI \
"${SCRIPT_DIR}/tiles.csv" \
  "${PROJECT_DIR}/boards/wwf_challenge.csv" 
