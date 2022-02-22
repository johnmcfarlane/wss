#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  easui_p \
  tiles.txt \
  "${PROJECT_DIR}/boards/wwf_challenge.txt" 
