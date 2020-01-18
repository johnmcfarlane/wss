#!/bin/bash

./src/play/play -rf /usr/bin

if [[ $? -eq 0 ]] ; then
    exit 1
fi
