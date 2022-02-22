#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$1

play \
  HVLDE_S \
  tiles.txt \
  "${PROJECT_DIR}/boards/wwf_challenge.txt" 
