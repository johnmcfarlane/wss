#!/bin/bash

# installs tools necessary to execute project scripts on Debian;
# tested on Buster and Ubuntu 19.04

set -e

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

echo Test Case 5

./src/play/play \
  begaz \
  "${SCRIPT_DIR}/tiles.csv" \
  "${SCRIPT_DIR}/../../../../boards/wwf_challenge.csv" \
     | diff "${SCRIPT_DIR}/expected.stdout" -

echo ...success
