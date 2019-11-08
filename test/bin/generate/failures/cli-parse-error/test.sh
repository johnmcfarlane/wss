#!/bin/bash

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

./src/generate/generate -rf /usr/bin

if [[ $? -eq 0 ]] ; then
    exit 1
fi
