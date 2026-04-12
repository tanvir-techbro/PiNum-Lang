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
