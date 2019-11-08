#!/bin/bash

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

./src/play/play \
  ? \
  "${SCRIPT_DIR}/tiles.csv" \
  "${SCRIPT_DIR}/irregular.csv"

if [[ $? -eq 0 ]] ; then
    exit 1
fi
