#!/bin/bash

shift

"$@" ./src/rack/rack -rf /usr/bin

if [[ $? -eq 0 ]] ; then
    exit 1
fi
