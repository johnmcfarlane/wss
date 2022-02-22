#!/usr/bin/env bash

set -uo pipefail

generate lexicon.txt lexicon expected_source

if [[ $? -ne 1 ]] ; then
    exit 1
fi
