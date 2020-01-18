#!/bin/bash

./src/rack/rack

if [[ $? -eq 0 ]] ; then
    exit 1
fi
