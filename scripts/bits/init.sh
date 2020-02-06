#!/bin/bash

# run first before config.sh to build dependencies

set -eo pipefail

BITS_DIR=$(cd "$(dirname "$0")"; pwd)

"$BITS_DIR"/init-conan.sh
"$BITS_DIR"/build-dependencies.sh
