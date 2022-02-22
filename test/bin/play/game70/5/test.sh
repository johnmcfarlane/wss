#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  EESK_XI \
  tiles.txt \
  "${PROJECT_DIR}/boards/wwf_challenge.txt" 
