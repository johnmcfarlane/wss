#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  NTJRRAU \
  tiles.txt \
  "${PROJECT_DIR}/boards/scrabble.txt" \
  --game=scrabble
