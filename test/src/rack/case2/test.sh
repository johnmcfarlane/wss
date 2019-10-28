#!/bin/bash

# installs tools necessary to execute project scripts on Debian;
# tested on Buster and Ubuntu 19.04

set -e

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

echo Test Case 2

./src/rack/rack \
  abcdefghijklmnopqrstuvwxyz? \
  --min-length 8 \
     | diff "${SCRIPT_DIR}/expected.stdout" -

echo ...success
