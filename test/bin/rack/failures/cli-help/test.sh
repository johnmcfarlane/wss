#!/bin/bash

shift

"$@" ./src/rack/rack --help

if [[ $? -eq 0 ]] ; then
    exit 1
fi
