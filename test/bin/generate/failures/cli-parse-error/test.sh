#!/bin/bash

shift

"$@" ./src/generate/generate one two many-arguments

if [[ $? -eq 0 ]] ; then
    exit 1
fi
