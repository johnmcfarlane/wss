#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$(cd "$(dirname "$0")"/../../../../..; pwd)

play qoihiaf tiles.txt "${PROJECT_DIR}/boards/wwf_regular.txt"
