#!/usr/bin/env bash

set -euo pipefail

shift

"$@" ./src/rack/rack \
  abcdefghijklmnopqrstuvwxyz? \
  --min-length 8
