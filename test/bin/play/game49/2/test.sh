#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  DGQIOAP \
  tiles.txt \
  "${PROJECT_DIR}/boards/wwf_regular.txt" 
