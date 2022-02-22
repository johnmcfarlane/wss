#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  fieht_x \
  tiles.txt \
  "${PROJECT_DIR}/boards/wwf_challenge.txt" 
