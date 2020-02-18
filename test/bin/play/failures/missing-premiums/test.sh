#!/bin/bash

set -eo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")"; pwd)
shift

"$@" ./src/play/play \
  "?" \
  "${SCRIPT_DIR}/tiles.txt"
