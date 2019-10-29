#!/bin/bash

set -e

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

./src/play/play \
  shorisq \
  "$1/words/wwf.txt" \
  "${SCRIPT_DIR}/tiles.csv" \
  "$1/boards/wwf_challenge.csv" 
