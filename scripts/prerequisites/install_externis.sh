#!/usr/bin/env bash
set -euo pipefail

# ==========================
# CONFIG
# ==========================

REPO_URL="https://github.com/royjacobson/externis.git"
REPO_NAME="externis"

TMP_DIR="$(mktemp -d)"

echo "Using temp dir: $TMP_DIR"

cp externis_fix.patch $TMP_DIR/

cd "$TMP_DIR"
git clone "$REPO_URL"
cd "$REPO_NAME"

git apply ../externis_fix.patch

cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Release

cmake --build build -j$(nproc)
cmake --install build

cd /
rm -rf "$TMP_DIR"

echo "externis installed successfully"
