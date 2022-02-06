#!/usr/bin/env bash

set -euo pipefail

if wordle ABBBB10000,ABBBB20000
then
  exit 1
fi
