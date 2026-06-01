#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"

typedef struct {
        token_list *tokens;
        size_t current;
} Parser;

// Core parser functions
ASTnode *parse(token_list *tokens);
// Internal parsing functions (recursive descent)
ASTnode *parse_program(Parser *parser);
ASTnode *parse_statement(Parser *parser);
ASTnode *parse_expression(Parser *parser);

// helper functions
// NOTE: below functions are defined in src/helper.c
static token peek(Parser *parser);
static token advance(Parser *parser);
static bool check(Parser *parser, tokenType type);
static bool match(Parser *parser, tokenType type);
static token consume(Parser *parser, tokenType type, const char *message);

#endif // !PARSER_H
