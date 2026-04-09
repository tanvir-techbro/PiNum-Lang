# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g -O3
SRC = src/main.c src/lexer.c src/parser.c src/codegen.c src/helper.c
TARGET = bin/pinum

# The default rule
all: $(TARGET)

# Rule to link the program
$(TARGET): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Test rules
TEST_SRC = tests/test_lexer.c src/lexer.c src/helper.c
TEST_TARGET = bin/test_lexer

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_TARGET)

# Rule to clean up the binary
clean:
	rm -f $(TARGET) $(TEST_TARGET)
