#!/bin/bash

shift

"$@" ./src/play/play --help

if [[ $? -ne 1 ]] ; then
    exit 1
fi
