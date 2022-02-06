#!/usr/bin/env bash

set -euo pipefail

if wordle "abcde00300,abcde00000"
then
  exit 1
fi
