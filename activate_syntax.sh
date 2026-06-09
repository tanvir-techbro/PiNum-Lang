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

# Define paths
NVIM_CONFIG="$HOME/.config/nvim"
REPO_DIR=$(pwd)

echo "Setting up PiNum syntax highlighting for Neovim..."

# Create directories if they don't exist
mkdir -p "$NVIM_CONFIG/ftdetect"
mkdir -p "$NVIM_CONFIG/syntax"

# Symlink instead of Copy
ln -sf "$REPO_DIR/extras/nvim/ftdetect/pinum.lua" "$NVIM_CONFIG/ftdetect/pinum.lua"
ln -sf "$REPO_DIR/extras/nvim/syntax/pinum.vim" "$NVIM_CONFIG/syntax/pinum.vim"

echo "Done."
