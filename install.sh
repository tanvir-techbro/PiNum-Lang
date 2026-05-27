#!/bin/bash

# PiNum-Lang Installation Script
# This script builds and installs PiNum-Lang locally.

set -e

REPO_URL="https://github.com/tanvir-techbro/PiNum-Lang"
INSTALL_DIR="$HOME/.pinum-lang"

echo "------------------------------------------"
echo "   Installing PiNum-Lang...               "
echo "------------------------------------------"

# 1. Check for dependencies
echo "Checking for GCC and Make..."
if ! command -v gcc &> /dev/null; then
    echo "Error: gcc not found. Please install a C compiler."
    exit 1
fi

if ! command -v make &> /dev/null; then
    echo "Error: make not found. Please install 'make'."
    exit 1
fi

# 2. Clone or Update
if [ -d "$INSTALL_DIR" ]; then
    echo "Updating existing installation in $INSTALL_DIR..."
    cd "$INSTALL_DIR"
    git pull
else
    echo "Cloning repository to $INSTALL_DIR..."
    git clone "$REPO_URL" "$INSTALL_DIR"
    cd "$INSTALL_DIR"
fi

# 3. Build
echo "Building PiNum..."
make

# 4. Install
echo "Installing to /usr/local/bin (requires sudo)..."
sudo make install

echo ""
echo "------------------------------------------"
echo "Successfully installed PiNum-Lang!"
echo "Try running: pinum"
echo "------------------------------------------"
