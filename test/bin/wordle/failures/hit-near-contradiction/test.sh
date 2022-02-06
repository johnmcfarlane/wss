#!/usr/bin/env bash

set -euo pipefail

if wordle Aaaaa20000,Aaaaa10000
then
  exit 1
fi
