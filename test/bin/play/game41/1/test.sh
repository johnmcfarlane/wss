#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  FSNEELI \
  tiles.txt \
  "${PROJECT_DIR}/boards/wwf_challenge.txt" 
