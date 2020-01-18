#!/bin/bash

./src/generate/generate --help

if [[ $? -eq 0 ]] ; then
    exit 1
fi
