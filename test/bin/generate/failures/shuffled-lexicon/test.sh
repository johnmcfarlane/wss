#!/bin/bash

set -e

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

./src/generate/generate ${SCRIPT_DIR}/lexicon.txt
