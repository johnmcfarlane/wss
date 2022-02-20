#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")"; pwd)
shift

"$@" ./src/generate/generate \
  "${SCRIPT_DIR}"/lexicon.txt lexicon \
  lexicon1

cat lexicon1.cpp lexicon1.h
