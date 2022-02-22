#!/usr/bin/env bash

set -uo pipefail

rack -rf /usr/bin

if [[ $? -ne 1 ]] ; then
    exit 1
fi
