#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  rruwtse \
  tiles.txt \
  "${PROJECT_DIR}/boards/scrabble.txt" \
  --game=scrabble
