#!/bin/bash

shift

"$@" ./src/rack/rack -rf /usr/bin

if [[ $? -ne 1 ]] ; then
    exit 1
fi
