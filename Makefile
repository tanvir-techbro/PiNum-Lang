# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g
SRC = src/main.c src/lexer.c src/parser.c src/codegen.c
TARGET = bin/pinum

# The default rule
all: $(TARGET)

# Rule to link the program
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Rule to clean up the binary
clean:
	rm -f $(TARGET)
