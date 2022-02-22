#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  EEIFARW \
  tiles.txt \
  "${PROJECT_DIR}/boards/wwf_regular.txt" 
