#!/usr/bin/env bash

set -uo pipefail

shift

"$@" ./src/generate/generate missing_file.txt

if [[ $? -ne 1 ]] ; then
    exit 1
fi
