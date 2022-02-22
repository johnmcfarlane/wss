#!/usr/bin/env bash

set -uo pipefail

generate lexicon.txt

if [[ $? -ne 1 ]] ; then
    exit 1
fi
