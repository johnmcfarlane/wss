#!/usr/bin/env bash

set -uo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")" || exit; pwd)

play \
  "?" \
  tiles.txt \
  "${SCRIPT_DIR}/premiums.txt"

if [[ $? -ne 1 ]] ; then
    exit 1
fi
