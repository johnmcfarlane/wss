#!/bin/bash

# run after config.sh and before tests to build targets

set -eo pipefail

NUM_CPUS=$(nproc)

cmake \
  --build . \
  "$@" \
    -- \
    --jobs "${NUM_CPUS}"
