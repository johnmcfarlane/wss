#!/bin/bash

shift

"$@" ./src/play/play one two many arguments

if [[ $? -ne 1 ]] ; then
    exit 1
fi
