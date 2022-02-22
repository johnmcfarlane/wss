#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")"; pwd)
PROJECT_DIR=$1
shift

"$@" ./src/play/play \
  NTJRRVI \
  "${SCRIPT_DIR}/tiles.txt" \
  "${PROJECT_DIR}/boards/scrabble.txt" \
  --game=scrabble
