#!/bin/bash

set -e

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

./src/rack/rack \
  abcdefghijklmnopqrstuvwxyz? \
  "$1/words/wwf.txt" \
  --min-length 8

