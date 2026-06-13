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

# This script is an automated test runner for PiNum-Lang.
# It allows running specific categories of tests (Lexer, AST, Parser, Codegen)
# and optionally checking for memory leaks using Valgrind.
#
# NOTE: Make sure to run "make" in the root directory before running this script
# to ensure the "bin/pinum" executable is up to date.

# ANSI Color Codes for enhanced terminal output
RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
CYAN='\033[1;36m'
NC='\033[0m' # No Color (Reset)

# --- Prerequisite Check ---
# Ensure the PiNum compiler binary exists before attempting to run tests
if [ ! -f "bin/pinum" ]; then
        echo -e "${RED}Error: bin/pinum not found.${NC}"
        echo -e "${YELLOW}Please run 'make' to build the project first.${NC}"
        exit 1
fi

# --- Core Testing Function ---
# logic: iterates through all .pn files in a directory and executes bin/pinum on them
# Arguments:
#   $1: Directory containing test files
#   $2: Valgrind flag ('y' to enable)
#   $3: Label for display (e.g., "Lexer")
run_tests() {
        local dir=$1
        local use_valgrind=$2
        local label=$3

        # Validate if the test directory actually exists
        if [ ! -d "$dir" ]; then
                echo -e "${RED}Directory $dir does not exist.${NC}"
                return
        fi

        # Glob all .pn files in the specified directory
        files=$(ls "$dir"/*.pn 2>/dev/null)
        if [ -z "$files" ]; then
                echo -e "${YELLOW}No tests found in $dir.${NC}"
                return
        fi

        echo -e "${CYAN}--- Running $label Tests ---${NC}"

        # Iterate and execute each test file
        for file in $files; do
                test_name=$(basename "$file")
                echo -e "${GREEN}Testing: $test_name${NC}"

                if [ "$use_valgrind" = "y" ]; then
                        # Run with Valgrind to check for memory leaks/errors
                        valgrind --leak-check=full --show-leak-kinds=all bin/pinum "$file"
                else
                        # Normal execution
                        bin/pinum "$file"
                fi
                echo -e "${CYAN}----------------------------------------------------${NC}"
                echo ""
        done
}

# --- Main UI and Interaction ---

# Clear screen for a clean user interface
clear

# Display ASCII Art Header
echo -e "${CYAN}"
cat <<'EOF'
 _____ _ _   _                       _
|  __ (_) \ | |                     | |
| |__) ||  \| |_   _ _ __ ___ ______| |     __ _ _ __   __ _
|  ___/ | . ` | | | | '_ ` _ \______| |    / _` | '_ \ / _` |
| |   | | |\  | |_| | | | | | |     | |___| (_| | | | | (_| |
|_|   |_|_| \_|\__,_|_| |_| |_|     |______\__,_|_| |_|\__, |
                                                        __/ |
                                                       |___/
EOF
echo -e "${NC}"
echo -e "${YELLOW}PiNum-Lang Test Runner${NC}"
echo "======================"
echo ""

# Menu selection
echo "Select which tests you want to run:"
echo "1) All tests"
echo "2) Lexer tests"
echo "3) AST tests"
echo "4) Parser tests"
echo "5) Codegen tests"
echo "6) Exit"
echo ""

read -p "Enter your choice [1-6]: " choice

# Handle Exit choice
if [ "$choice" -eq 6 ]; then
        echo "Exiting."
        exit 0
fi

# Input validation for menu choice
if [[ ! "$choice" =~ ^[1-5]$ ]]; then
        echo -e "${RED}Invalid choice. Exiting.${NC}"
        exit 1
fi

echo ""
# Prompt for Valgrind usage
read -p "Do you want to run tests with Valgrind? (recommended) [y/n]: " valgrind_choice
# Convert input to lowercase for consistency
valgrind_choice=$(echo "$valgrind_choice" | tr '[:upper:]' '[:lower:]')

# --- Dependency Check for Valgrind ---
if [ "$valgrind_choice" = "y" ]; then
        if ! command -v valgrind &>/dev/null; then
                echo -e "${RED}Error: valgrind is not installed. Please install it or run without it.${NC}"
                exit 1
        fi
fi

echo ""

# --- Execute selected tests based on user input ---
case $choice in
1)
        run_tests "test/lexer-tests" "$valgrind_choice" "Lexer"
        run_tests "test/ast-tests" "$valgrind_choice" "AST"
        run_tests "test/parser-tests" "$valgrind_choice" "Parser"
        run_tests "test/codegen-tests" "$valgrind_choice" "Codegen"
        ;;
2)
        run_tests "test/lexer-tests" "$valgrind_choice" "Lexer"
        ;;
3)
        run_tests "test/ast-tests" "$valgrind_choice" "AST"
        ;;
4)
        run_tests "test/parser-tests" "$valgrind_choice" "Parser"
        ;;
5)
        run_tests "test/codegen-tests" "$valgrind_choice" "Codegen"
        ;;
esac

echo -e "${GREEN}Test execution finished.${NC}"
