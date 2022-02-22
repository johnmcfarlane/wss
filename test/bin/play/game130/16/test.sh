#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  ELAORJA \
  tiles.txt \
  "${PROJECT_DIR}/boards/wwf_regular.txt" 
