#!/usr/bin/env bash

set -uo pipefail

generate --help

if [[ $? -ne 1 ]] ; then
    exit 1
fi
