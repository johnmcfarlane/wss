#!/usr/bin/env bash

set -uo pipefail

shift

"$@" ./src/generate/generate --help

if [[ $? -ne 1 ]] ; then
    exit 1
fi
