#!/bin/bash

shift

"$@" ./src/play/play -rf /usr/bin

if [[ $? -ne 1 ]] ; then
    exit 1
fi
