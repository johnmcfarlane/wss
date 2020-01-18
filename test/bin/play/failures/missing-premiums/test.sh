#!/bin/bash

set -eo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")"; pwd)

./src/play/play \
  "?" \
  "${SCRIPT_DIR}/tiles.csv"
