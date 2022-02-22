#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  DTVER_Z \
  tiles.txt \
  "${PROJECT_DIR}/boards/wwf_regular.txt" 
