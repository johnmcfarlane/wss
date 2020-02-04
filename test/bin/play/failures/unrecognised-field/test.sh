#!/bin/bash

SCRIPT_DIR=$(cd "$(dirname "$0")" || exit; pwd)
shift

"$@" ./src/play/play \
  "?" \
  "${SCRIPT_DIR}/tiles.csv" \
  "${SCRIPT_DIR}/irregular.csv"

if [[ $? -ne 1 ]] ; then
    exit 1
fi
