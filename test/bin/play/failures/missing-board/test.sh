#!/bin/bash

# installs tools necessary to execute project scripts on Debian;
# tested on Buster and Ubuntu 19.04

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

./src/play/play \
  ? \
  "${SCRIPT_DIR}/tiles.csv" \
  "${SCRIPT_DIR}/irregular.csv"

if [[ $? -eq 0 ]] ; then
    exit 1
fi
