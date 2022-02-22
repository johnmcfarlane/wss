#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  EIEHUPO \
  tiles.txt \
  "${PROJECT_DIR}/boards/scrabble.txt" \
  --game=scrabble
