#!/usr/bin/env bash
set -euo pipefail

: "${ALLOWED_PLATFORMS}"

if [ -f /etc/os-release ]; then
    . /etc/os-release
else
    echo "/etc/os-release not found, cannot detect OS. Exiting."
    exit 1
fi

for platform in $ALLOWED_PLATFORMS; do
    if [ "$ID" = "$platform" ]; then
        echo "Detected allowed platform: $NAME ($ID)"
        exit 0
    fi
done

echo "This script only supports the following platforms: $ALLOWED_PLATFORMS. Detected: $NAME ($ID)"
exit 1
