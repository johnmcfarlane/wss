#!/bin/bash

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

./src/rack/rack --help

if [[ $? -eq 0 ]] ; then
    exit 1
fi
