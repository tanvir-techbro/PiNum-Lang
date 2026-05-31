# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -O3
SRC = src/main.c src/lexer.c src/lexer_filter.c src/parser.c src/ast.c src/codegen.c src/helper.c
VERSION = $(shell cat VERSION)

# OS detection
ifeq ($(OS),Windows_NT)
    TARGET = bin/pinum.exe
    TEST_TARGET = bin/test_lexer.exe
    MKDIR = if not exist bin mkdir bin
    RM = del /Q
    INSTALL_PATH = $(USERPROFILE)\bin
    CP = copy /Y
else
    TARGET = bin/pinum
    TEST_TARGET = bin/test_lexer
    MKDIR = mkdir -p bin
    RM = rm -f
    CP = cp
    # Check for Termux
    ifneq ($(wildcard /data/data/com.termux/files/usr/bin/*),)
        INSTALL_PATH ?= $(PREFIX)/bin
    else
        INSTALL_PATH ?= /usr/local/bin
    endif
endif

# The default rule
all: $(TARGET)

# Compile it to PiNum-Lang/bin/ directory
$(TARGET): $(SRC)
	@$(MKDIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# To install it locally
install: $(TARGET)
ifeq ($(OS),Windows_NT)
	@if not exist "$(INSTALL_PATH)" mkdir "$(INSTALL_PATH)"
	$(CP) bin\pinum.exe "$(INSTALL_PATH)\pinum.exe"
	@echo "Installed to $(INSTALL_PATH). Please ensure this directory is in your PATH."
else
	mv $(TARGET) $(INSTALL_PATH)/
endif

# Test rules
TEST_SRC = tests/test_lexer.c src/lexer.c src/helper.c

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC)
	@$(MKDIR)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_TARGET)

# Rule to clean up the binary
clean:
	$(RM) $(TARGET) $(TEST_TARGET)

# Neovim syntax activation
nvim:
	@$(MKDIR)
	# $(CC) $(CFLAGS) $(SRC) -o $(TARGET)
ifeq ($(OS),Windows_NT)
	@echo "Manual setup required for Neovim on Windows. Copy extras/nvim/ to your AppData/Local/nvim folder."
else
	chmod +x activate_syntax.sh && ./activate_syntax.sh
endif

.PHONY: all test clean nvim install
