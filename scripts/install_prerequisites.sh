#!/usr/bin/env bash

set -euo pipefail

ALLOWED_PLATFORMS="ubuntu debian" 

ALLOWED_PLATFORMS=$ALLOWED_PLATFORMS ./scripts/verify_platform.sh

apt-get update
apt-get install -y  \
    wget            \
    tar             \
    cmake           \
    graphviz

./scripts/prerequisites/install_doxygen.sh
