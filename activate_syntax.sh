#!/bin/bash

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
