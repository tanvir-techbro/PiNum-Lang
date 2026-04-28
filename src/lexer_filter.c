// NOTE: Different filter functions in include/lexer.h will be defined here.
// These functions are used to filter unusual tokens and refine tokens.

#include "../include/lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Keeps coments from being listed in the token list.
// Whatever is after a hashtag outside print or any wtring function, will be treated as a comment.
// We only support 1 line comment.
token token_ignore_comment(token t, FILE *buffer) {
        while (t.type != TOKEN_NLINE && t.type != TOKEN_EOF) {
                if (t.value) {
                        free(t.value);
                }
                t = lexer_tokenizer(buffer);
        }
        return t;
}

// Turns all the tokens inside a quote with a TOKEN_ID
// This function reads everything between the quotes and stores it in a single TOKEN_ID.
token token_quote_handler(token t, FILE *buffer) {
        char quote_char = (t.type == TOKEN_SQUOTE) ? '\'' : '\"';

        int size = 0;
        int capacity = 16;
        char *content = malloc(capacity);

        // Add the opening quote
        content[size++] = quote_char;

        int ch = fgetc(buffer);

        // Read characters until we find the closing quote or hit EOF
        while (ch != EOF && ch != quote_char) {
                if (size + 2 >= capacity) { // +2 to ensure space for closing quote and \0
                        capacity *= 2;
                        content = realloc(content, capacity);
                }
                content[size++] = (char)ch;
                ch = fgetc(buffer);
        }

        // Add the closing quote if found
        if (ch == quote_char) {
                content[size++] = quote_char;
        }
        content[size] = '\0';

        // Free the initial quote character string (" or ')
        if (t.value) {
                free(t.value);
        }

        // Update the token to be a TOKEN_QUOTED_STRING with the captured string content
        t.type = TOKEN_QUOTED_STRING;
        t.value = content;

        // If we hit EOF before finding the closing quote, mark it as EOF
        if (ch == EOF) {
                t.type = TOKEN_EOF;
        }

        return t;
}
