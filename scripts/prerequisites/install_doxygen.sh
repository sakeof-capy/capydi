#!/usr/bin/env bash
set -euo pipefail

DOXYGEN_VERSION="1.15.0"
DOXYGEN_VERSION_UNDERSCORE="${DOXYGEN_VERSION//./_}"
INSTALL_DIR="/usr/local"
TARBALL="doxygen-${DOXYGEN_VERSION}.linux.bin.tar.gz"
URL="https://github.com/doxygen/doxygen/releases/download/Release_${DOXYGEN_VERSION_UNDERSCORE}/${TARBALL}"

echo "Installing Doxygen ${DOXYGEN_VERSION}..."

if command -v doxygen >/dev/null 2>&1; then
    INSTALLED_VERSION=$(doxygen --version | awk '{print $1}')
    if [ "$INSTALLED_VERSION" = "$DOXYGEN_VERSION" ]; then
        echo "Doxygen ${DOXYGEN_VERSION} is already installed."
        exit 0
    else
        echo "Different Doxygen version installed: ${INSTALLED_VERSION}. Updating..."
    fi
else
    echo "Doxygen not found. Installing..."
fi

cd /tmp
rm -f "$TARBALL"
wget -c "$URL" -O "$TARBALL"

tar -xzf "$TARBALL" -C "$INSTALL_DIR"
rm "$TARBALL"

ln -sf "${INSTALL_DIR}/doxygen-${DOXYGEN_VERSION}/bin/doxygen" /usr/local/bin/doxygen

doxygen --version
echo "Doxygen ${DOXYGEN_VERSION} installed successfully!"
