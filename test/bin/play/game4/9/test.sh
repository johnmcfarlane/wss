#!/bin/bash

# installs tools necessary to execute project scripts on Debian;
# tested on Buster and Ubuntu 19.04

set -e

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

# TODO: Should be 72, not 80, because L is a blank.
./src/play/play \
  zpviall \
  "$1/words/wwf.txt" \
  "${SCRIPT_DIR}/tiles.csv" \
  "$1/boards/wwf_challenge.csv" 
