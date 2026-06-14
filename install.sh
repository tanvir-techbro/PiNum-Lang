#!/bin/bash

#******************************************************************#
#   _____ _ _   _                       _                          #
#  |  __ (_) \ | |                     | |                         #
#  | |__) ||  \| |_   _ _ __ ___ ______| |     __ _ _ __   __ _    #
#  |  ___/ | . ` | | | | '_ ` _ \______| |    / _` | '_ \ / _` |   #
#  | |   | | |\  | |_| | | | | | |     | |___| (_| | | | | (_| |   #
#  |_|   | |_| \_|\__,_|_| |_| |_|     |______\__,_|_| |_|\__, |   #
#                                                          __/ |   #
#                                                         |___/    #
#                                                                  #
#  Copyright (c) 2026 tanvir-techbro.                              #
#  You may opt to use, copy, modify, merge, publish, distribute    #
#  and/or sell copies of the Software, and permit persons to whom  #
#  the Software is furnished to do so, under the conditions of the #
#  LICENSE.                                                        #
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, #
#  EXPRESS OR IMPLIED.                                             #
#                                                                  #
#  If you find any bug you would be highly encouraged to create a  #
#  github issue at <https://github.com/tanvir-techbro/PiNum-Lang>  #
#  or contact <surjointelligence.team@gmail.com>                   #
#******************************************************************#

# PiNum-Lang Installation Script
# This script builds and installs PiNum-Lang locally.
# Requirements:
# - C Compiler (GCC 4.8+, Clang 3.5+, or any C99+ compliant compiler)
# - Make
# - POSIX environment (for strdup support)

set -e

REPO_URL="https://github.com/tanvir-techbro/PiNum-Lang"
INSTALL_DIR="$HOME/.pinum-lang"

# 1. Check for dependencies
echo "Checking for a C compiler and Make..."

# Try to find a usable C compiler
if command -v cc &>/dev/null; then
        CC_BIN="cc"
elif command -v gcc &>/dev/null; then
        CC_BIN="gcc"
elif command -v clang &>/dev/null; then
        CC_BIN="clang"
else
        echo "Error: No C compiler found (cc, gcc, or clang). Please install one."
        exit 1
fi

# Print compiler version for debugging/confirmation
$CC_BIN --version | head -n 1

if ! command -v make &>/dev/null; then
        echo "Error: make not found. Please install 'make'."
        exit 1
fi

# 2. Clone or Update
if [ -d "$INSTALL_DIR" ]; then
        echo "Updating existing installation in $INSTALL_DIR..."
        cd "$INSTALL_DIR"
        git checkout . --quiet
        git pull --quiet
else
        echo "-----------------------------"
        echo "   Installing PiNum-Lang...  "
        echo "-----------------------------"
        echo ""
        echo "Cloning repository to $INSTALL_DIR..."
        git clone --quiet "$REPO_URL" "$INSTALL_DIR"
        cd "$INSTALL_DIR"
fi

# 3. Build
echo "Building PiNum..."
make -s CC=$CC_BIN >/dev/null

# 4. Install
# Checking for termux; sudo wouldn't run on termux
# if it is not termux then it is a unix based OS.
if [ -d "/data/data/com.termux" ] || [ -n "$TERMUX_VERSION" ]; then
        echo "Termux detected! Installing to $PREFIX/bin..."
        make -s install >/dev/null
else
        echo "Installing to /usr/local/bin (requires sudo)..."
        sudo make -s install >/dev/null
fi

# 5. Neovim Syntax (Optional)
# detects for neovim and runs activate_syntax.sh
if [ -d "$HOME/.config/nvim" ] || [ -d "$HOME/.local/share/nvim" ]; then
        echo ""
        echo "Neovim detected!"
        # When piped from curl, stdin is the pipe. We need to read from the terminal (/dev/tty).
        read -p "Do you want to activate PiNum syntax highlighting for Neovim? (y/n): " -n 1 -r </dev/tty
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
                echo "Activating Neovim syntax..."
                make -s nvim >/dev/null
        fi
fi

# deleting the temporary build folder
rm -rf src include example test bin .gitignore .clang-format Makefile CONTRIBUTING.md install.sh runtime/.gitkeep

echo ""
echo "----------------------------------"
echo "Successfully installed PiNum-Lang!"
echo "Try running: pinum"
echo "----------------------------------"
