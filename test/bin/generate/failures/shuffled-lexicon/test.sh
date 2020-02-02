#!/bin/bash

set -eo pipefail

SCRIPT_DIR=$(cd "$(dirname "$0")"; pwd)
shift

"$@" ./src/generate/generate "${SCRIPT_DIR}"/lexicon.txt
