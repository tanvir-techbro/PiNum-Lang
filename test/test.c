#include "../include/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_buffer_overflow() {
    printf("Testing buffer overflow in lexer_tokenize_numbers...\n");
    FILE *f = fopen("overflow.pn", "w");
    // Write 100 digits, which is more than the 64 byte buffer in lexer_tokenize_numbers
    for (int i = 0; i < 100; i++) {
        fputc('1', f);
    }
    fclose(f);

    f = fopen("overflow.pn", "r");
    // This should crash or corrupt memory if there's no bounds check
    token t = lexer_tokenize_numbers(f);
    printf("Tokenized long number. Type: %d\n", t.type);
    fclose(f);
    remove("overflow.pn");
}

int main() {
    test_buffer_overflow();
    return 0;
}
