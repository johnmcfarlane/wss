#!/usr/bin/env bash

set -euo pipefail

if wordle "abcde0000"
then
  exit 1
fi
