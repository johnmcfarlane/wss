#!/bin/bash

# the second half of init.sh

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/../..; pwd)

conan install \
  --build=missing "${PROJECT_DIR}" \
  --profile=./conan-profile
