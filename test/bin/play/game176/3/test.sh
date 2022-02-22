#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  s_za_ns \
  tiles.txt \
  "${PROJECT_DIR}/boards/wwf_challenge.txt"
