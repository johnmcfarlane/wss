#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  DNQV_TS \
  tiles.txt \
  "${PROJECT_DIR}/boards/wwf_challenge.txt" 
