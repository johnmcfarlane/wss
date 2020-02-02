#!/bin/bash

set -eo pipefail

shift

"$@" ./src/rack/rack \
  abcdefghijklmnopqrstuvwxyz? \
  --min-length 8
