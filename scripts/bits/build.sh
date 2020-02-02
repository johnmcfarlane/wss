#!/bin/bash

# run after config.sh and before tests to build targets

set -eo pipefail

NUM_CPUS=$(nproc)

echo Before build:
ccache --show-stats

cmake \
  --build . \
  "$@" \
    -- \
    --jobs "${NUM_CPUS}"

echo After build:
ccache --show-stats
