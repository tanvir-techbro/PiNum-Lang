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

# Variables
CC ?= cc
CFLAGS += -Wall -Wextra -g -O3
SRC = src/main.c src/lexer.c src/lexer_filter.c src/parser.c src/ast.c src/codegen.c src/helper.c src/error.c
VERSION = $(shell cat VERSION)

TARGET = bin/pinum
TEST_TARGET = bin/test_lexer
MKDIR = mkdir -p bin
RM = rm -f

# Check for Termux
ifneq ($(wildcard /data/data/com.termux/files/usr/bin/*),)
    INSTALL_PATH ?= $(PREFIX)/bin
else
    INSTALL_PATH ?= /usr/local/bin
endif

# The default rule
all: $(TARGET)

# Compile it to PiNum-Lang/bin/ directory
$(TARGET): $(SRC)
	@$(MKDIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# To install it locally
install: $(TARGET)
	mv $(TARGET) $(INSTALL_PATH)/

# Rule to clean up the binary
clean:
	$(RM) $(TARGET) $(TEST_TARGET)

# Neovim syntax activation
nvim:
	@$(MKDIR)
	chmod +x activate_syntax.sh && ./activate_syntax.sh

.PHONY: all test clean nvim install
