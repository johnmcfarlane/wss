#!/bin/bash

./src/play/play

if [[ $? -eq 0 ]] ; then
    exit 1
fi
