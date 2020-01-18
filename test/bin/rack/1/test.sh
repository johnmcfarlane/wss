#!/bin/bash

set -eo pipefail

./src/rack/rack \
  abcdefghijklmnopqrstuvwxyz? \
  --min-length 8
