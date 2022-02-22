#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  DLT_SIA \
  tiles.txt \
  "${PROJECT_DIR}/boards/wwf_regular.txt" 
