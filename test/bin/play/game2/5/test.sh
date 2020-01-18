#!/bin/bash

set -e

SCRIPT_DIR=$(cd "$(dirname "$0")"; pwd)

./src/play/play \
  begaz \
"${SCRIPT_DIR}/tiles.csv" \
  "$1/boards/wwf_challenge.csv"
