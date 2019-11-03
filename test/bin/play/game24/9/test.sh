#!/bin/bash

# installs tools necessary to execute project scripts on Debian;
# tested on Buster and Ubuntu 19.04

set -e

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

./src/play/play \
  iitgdht \
"${SCRIPT_DIR}/tiles.csv" \
  "$1/boards/wwf_regular.csv" 
