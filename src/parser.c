#include "../include/parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// NOTE: This parser uses recursive decent parsing method.

// ---------------------------- //
// --- Core functionalities --- //
// ---------------------------- //

// --- initialization function (main) ---
ASTnode *parse(token_list *tokens) {
        Parser parser = {tokens, 0};
        return parse_program(&parser);
}

// --- Internal parsing functions (using recursive decent) ---
ASTnode *parse_program(Parser *parser) {
        ASTnode *program = create_ast_node(NODE_PROGRAM);
        while (!check(parser, TOKEN_EOF)) {
                // Skip newlines between statements
                if (match(parser, TOKEN_NLINE))
                        continue;

                ast_add_statement(program, parse_statement(parser));
        }
        return program;
}
ASTnode *parse_statement(Parser *parser) {
        if (match(parser, TOKEN_PRINT)) {
                ASTnode *expression = parse_expression(parser);
                consume_end_of_statement(parser);
                ASTnode *node = create_ast_node(NODE_PRINT);
                node->data.print.expression = expression;
                return node;
        }

        // Fallback to expression statement
        ASTnode *expression = parse_expression(parser);
        consume_end_of_statement(parser);
        return expression;

        // TODO: add more statements to be parsed
}
ASTnode *parse_expression(Parser *parser) {
        // --- Literals parsing ---
        // Intager node parsing
        if (match(parser, TOKEN_INUM)) {
                return make_int_node(parser->tokens->tokens[parser->current - 1].int_value);
        }
        // Floting point node parsing
        if (match(parser, TOKEN_FNUM)) {
                return make_float_node(parser->tokens->tokens[parser->current - 1].float_value);
        }
        // string node parsing
        if (match(parser, TOKEN_STRING)) {
                return make_string_node(parser->tokens->tokens[parser->current - 1].value);
        }
        // boolean node parsing
        if (match(parser, TOKEN_BOOL)) {
                return make_bool_node(parser->tokens->tokens[parser->current - 1].value);
        }
        // Identifier node parsing
        if (match(parser, TOKEN_ID)) {
                return make_identifier_node(parser->tokens->tokens[parser->current - 1].value);
        }

        // Error
        fprintf(stderr, "Parser error: Unexpected token %s\n", peek(parser).value);
        exit(EXIT_FAILURE);
}
