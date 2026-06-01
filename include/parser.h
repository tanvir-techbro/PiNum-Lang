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
token peek(Parser *parser);
token advance(Parser *parser);
bool check(Parser *parser, tokenType type);
bool match(Parser *parser, tokenType type);
token consume(Parser *parser, tokenType type, const char *message);
void consume_end_of_statement(Parser *parser);

#endif // !PARSER_H
