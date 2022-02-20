#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")"; pwd)
shift

"$@" ./src/generate/generate \
  "${SCRIPT_DIR}"/lexicon.txt lexicon \
  lexicon

cat lexicon.cpp lexicon.h
