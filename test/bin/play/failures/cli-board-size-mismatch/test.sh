#!/bin/bash

SCRIPT_DIR=$(cd "$(dirname "$0")" || exit; pwd)

./src/play/play \
  "?" \
  "${SCRIPT_DIR}/tiles.csv" \
  "${SCRIPT_DIR}/premiums.csv"

if [[ $? -eq 0 ]] ; then
    exit 1
fi
