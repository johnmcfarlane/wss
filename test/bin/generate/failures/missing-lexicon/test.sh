#!/bin/bash

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

./src/generate/generate missing_file.txt

if [[ $? -eq 0 ]] ; then
    exit 1
fi
