#!/usr/bin/env bash

set -euo pipefail

if wordle "abcde00!00"
then
  exit 1
fi
