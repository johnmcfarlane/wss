#!/bin/bash

set -e

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

./src/play/play \
  oqo_ehs \
  "$1/words/wwf.txt" \
  "${SCRIPT_DIR}/tiles.csv" \
  "$1/boards/wwf_challenge.csv" 
