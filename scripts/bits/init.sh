#!/bin/bash

# run first before config.sh to build dependencies

set -eo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/../..; pwd)

pip3 install --quiet conan
conan remote add --force my-conan-repo https://api.bintray.com/conan/bincrafters/public-conan
conan profile new ./conan-profile --detect --force
conan install \
  --build=missing "${PROJECT_DIR}" \
  --profile=./conan-profile
