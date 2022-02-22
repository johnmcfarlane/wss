#!/usr/bin/env bash

set -uo pipefail

play abcd

if [[ $? -ne 1 ]] ; then
    exit 1
fi
