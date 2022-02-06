#!/usr/bin/env bash

set -euo pipefail

if wordle a b
then
  exit 1
fi
