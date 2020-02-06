#!/bin/bash

# the first half of init.sh

set -eo pipefail

pip3 install --quiet conan
conan remote add --force my-conan-repo https://api.bintray.com/conan/bincrafters/public-conan
conan profile new ./conan-profile --detect --force
