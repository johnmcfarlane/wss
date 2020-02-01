#!/bin/bash

# installs tools necessary to execute project scripts on Debian;
# tested on Buster and Ubuntu 19.04

set -eo pipefail

apt update --yes --quiet
apt upgrade --yes --quiet
apt install --yes --quiet ccache cmake cppcheck lcov python3-pip
ccache --show-stats
