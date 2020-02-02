#!/bin/bash

shift

"$@" ./src/play/play abcd

if [[ $? -eq 0 ]] ; then
    exit 1
fi
