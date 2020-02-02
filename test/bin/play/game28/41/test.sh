#!/bin/bash

set -eo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")"; pwd)

./src/play/play \
  VUYCIEL \
  "${SCRIPT_DIR}/tiles.csv" \
  "$1/boards/wwf_regular.csv" 
