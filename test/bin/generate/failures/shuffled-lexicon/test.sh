#!/bin/bash

set -e

# installs tools necessary to execute project scripts on Debian;
# tested on Buster and Ubuntu 19.04

SCRIPT_DIR=$(cd $(dirname "$0"); pwd)

./src/generate/generate ${SCRIPT_DIR}/lexicon.txt
