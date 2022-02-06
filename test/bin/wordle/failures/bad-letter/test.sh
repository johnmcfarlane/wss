#!/usr/bin/env bash

set -euo pipefail

if wordle "abcd_00000"
then
  exit 1
fi
