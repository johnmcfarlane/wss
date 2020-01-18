#!/bin/bash

set -e

SCRIPT_DIR=$(cd "$(dirname "$0")"; pwd)

./src/play/play \
  VUYAODE \
"${SCRIPT_DIR}/tiles.csv" \
  "$1/boards/wwf_regular.csv" 
