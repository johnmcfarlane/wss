#!/usr/bin/env bash

set -uo pipefail

play \
  "?" \
  tiles.txt \
  irregular.txt

if [[ $? -ne 1 ]] ; then
    exit 1
fi
