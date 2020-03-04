#!/bin/bash

BUILD_DIR=$(pwd)
SCRIPT_DIR=$(cd "$(dirname "$0")" || exit; pwd)
shift

cd "${SCRIPT_DIR}" || exit

"$@" "${BUILD_DIR}/src/play/play" \
  "?" \
  tiles.txt \
  premiums.txt

if [[ $? -ne 1 ]] ; then
    exit 1
fi
