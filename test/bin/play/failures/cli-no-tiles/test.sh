#!/bin/bash

shift

"$@" ./src/play/play abcd

if [[ $? -ne 1 ]] ; then
    exit 1
fi
