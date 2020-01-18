#!/bin/bash

./src/play/play --help

if [[ $? -eq 0 ]] ; then
    exit 1
fi
