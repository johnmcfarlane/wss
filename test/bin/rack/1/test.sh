#!/bin/bash

set -e

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

./src/rack/rack \
  abcdefghijklmnopqrstuvwxyz? \
  --min-length 8
