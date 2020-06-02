#!/bin/bash

# installs tools necessary to execute project scripts on Debian;
# tested on Buster and Ubuntu 19.04

set -eo pipefail

apt update --yes --quiet
apt upgrade --yes --quiet
apt install --auto-remove --no-install-recommends --yes --quiet "$@"
ccache --show-stats
