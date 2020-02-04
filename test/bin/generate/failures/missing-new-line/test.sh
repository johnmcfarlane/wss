#!/bin/bash

SCRIPT_DIR=$(cd "$(dirname "$0")" || exit; pwd)
shift

"$@" ./src/generate/generate "${SCRIPT_DIR}"/lexicon.txt

if [[ $? -ne 1 ]] ; then
    exit 1
fi
