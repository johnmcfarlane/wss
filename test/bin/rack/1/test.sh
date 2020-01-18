#!/bin/bash

set -e

./src/rack/rack \
  abcdefghijklmnopqrstuvwxyz? \
  --min-length 8
