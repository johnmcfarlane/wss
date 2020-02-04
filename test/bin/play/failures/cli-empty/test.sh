#!/bin/bash

shift

"$@" ./src/play/play

if [[ $? -ne 1 ]] ; then
    exit 1
fi
