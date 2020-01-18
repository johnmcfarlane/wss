#!/bin/bash

./src/generate/generate -rf /usr/bin

if [[ $? -eq 0 ]] ; then
    exit 1
fi
