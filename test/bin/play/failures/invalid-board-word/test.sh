#!/bin/bash

set -eo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")"; pwd)
PROJECT_DIR=$1
shift

set +e
"$@" ./src/play/play \
  keluoee \
  "${SCRIPT_DIR}/tiles.txt" \
  "${PROJECT_DIR}/boards/wwf_challenge.txt"

if [[ $? -ne 1 ]] ; then
    exit 1
fi
