#!/bin/bash

shift

"$@" ./src/rack/rack

if [[ $? -ne 1 ]] ; then
    exit 1
fi
