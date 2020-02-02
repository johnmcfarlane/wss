#!/bin/bash

shift

"$@" ./src/rack/rack z

if [[ $? -eq 0 ]] ; then
    exit 1
fi
