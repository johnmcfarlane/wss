#!/usr/bin/env bash

set -uo pipefail

BUILD_DIR=$(pwd)

shift

if "$@" "${BUILD_DIR}/src/generate/generate" a b c d e
then
    exit 1
fi
