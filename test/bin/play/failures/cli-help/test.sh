#!/usr/bin/env bash

set -uo pipefail

play --help

if [[ $? -ne 1 ]] ; then
    exit 1
fi
