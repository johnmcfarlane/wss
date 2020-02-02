#!/bin/bash

shift

"$@" ./src/rack/rack

if [[ $? -eq 0 ]] ; then
    exit 1
fi
