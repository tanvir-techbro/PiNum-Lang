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
if ! command -v gcc &>/dev/null; then
        echo "Error: gcc not found. Please install a C compiler."
        exit 1
fi

if ! command -v make &>/dev/null; then
        echo "Error: make not found. Please install 'make'."
        exit 1
fi

# 2. Clone or Update
if [ -d "$INSTALL_DIR" ]; then
        echo "Updating existing installation in $INSTALL_DIR..."
        cd "$INSTALL_DIR"
        git pull --quiet
else
        echo "Cloning repository to $INSTALL_DIR..."
        git clone --quiet "$REPO_URL" "$INSTALL_DIR"
        cd "$INSTALL_DIR"
fi

# 3. Build
echo "Building PiNum..."
make

# 4. Install
echo "Installing to /usr/local/bin (requires sudo)..."
sudo make install

# 5. Neovim Syntax (Optional)
if [ -d "$HOME/.config/nvim" ] || [ -d "$HOME/.local/share/nvim" ]; then
        echo ""
        echo "Neovim detected!"
        # When piped from curl, stdin is the pipe. We need to read from the terminal (/dev/tty).
        read -p "Do you want to activate PiNum syntax highlighting for Neovim? (y/n): " -n 1 -r </dev/tty
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
                echo "Activating Neovim syntax..."
                make nvim
        fi
fi

echo ""
echo "------------------------------------------"
echo "Successfully installed PiNum-Lang!"
echo "Try running: pinum"
echo "------------------------------------------"
