#include "../include/parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// NOTE: This parser uses recursive decent parsing method.

// --- Helper Functions ---
// Peeks/takes a looks at the current token without moving to the next token.
static token peek(Parser *parser) {
        if (parser->current >= parser->tokens->size) {
                return parser->tokens->tokens[parser->tokens->size - 1];
        }
        return parser->tokens->tokens[parser->current];
}
// moves to the next token
static token advance(Parser *parser) {
        if (parser->current < parser->tokens->size) {
                parser->current++;
        }
        return parser->tokens->tokens[parser->current - 1];
}
// checks the current token type
static bool check(Parser *parser, tokenType type) {
        return peek(parser).type == type;
}
// checks if current token matches with another token and moves on
static bool match(Parser *parser, tokenType type) {
        if (check(parser, type)) {
                advance(parser);
                return true;
        }
        // return false if it dosent match and stays at the same place (dosent move on)
        return false;
}
// checks for error and throws error messages
static token consume(Parser *parser, tokenType type, const char *message) {
        if (check(parser, type)) {
                return advance(parser);
        }
        fprintf(stderr, "Parser error: %s", message);
        exit(EXIT_FAILURE);
}

// --- Core functionalities ---
// initialization
ASTnode *parse(token_list *tokens) {
        Parser parser = {tokens, 0};
        return parse_program(&parser);
}
