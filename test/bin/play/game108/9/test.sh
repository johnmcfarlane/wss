#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  QAEI_J_ \
  tiles.txt \
  "${PROJECT_DIR}/boards/wwf_challenge.txt" 
