#!/bin/bash

# installs tools necessary to execute project scripts on Debian;
# tested on Buster and Ubuntu 19.04

set -e

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

./src/rack/rack \
  abcdefghijklmnopqrstuvwxyz? \
  "$1/words/wwf.txt" \
  --min-length 8

