#!/bin/bash

./src/generate/generate missing_file.txt

if [[ $? -eq 0 ]] ; then
    exit 1
fi
