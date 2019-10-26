#!/bin/bash

# installs tools necessary to execute project scripts on Debian;
# tested on Buster and Ubuntu 19.04

set -e

apt update --yes --quiet
apt upgrade --yes --quiet
apt install --yes --quiet ccache clang-tidy cmake python3-pip
