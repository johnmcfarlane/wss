#!/bin/bash

set -e

SCRIPT_DIR=$(cd "$(dirname "$0")"; pwd)

# TODO: Should be 72, not 80, because L is a blank.
./src/play/play \
  zpviall \
"${SCRIPT_DIR}/tiles.csv" \
  "$1/boards/wwf_challenge.csv" 
