#!/bin/bash

shift

"$@" ./src/rack/rack z

if [[ $? -ne 1 ]] ; then
    exit 1
fi
