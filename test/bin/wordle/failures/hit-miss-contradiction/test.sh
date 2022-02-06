#!/usr/bin/env bash

set -euo pipefail

if wordle Aaaaa20000,Aaaaa00000
then
  exit 1
fi
