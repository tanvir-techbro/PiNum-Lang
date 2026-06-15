/********************************************************************
 *   _____ _ _   _                       _                          *
 *  |  __ (_) \ | |                     | |                         *
 *  | |__) ||  \| |_   _ _ __ ___ ______| |     __ _ _ __   __ _    *
 *  |  ___/ | . ` | | | | '_ ` _ \______| |    / _` | '_ \ / _` |   *
 *  | |   | | |\  | |_| | | | | | |     | |___| (_| | | | | (_| |   *
 *  |_|   | |_| \_|\__,_|_| |_| |_|     |______\__,_|_| |_|\__, |   *
 *                                                          __/ |   *
 *                                                         |___/    *
 *                                                                  *
 *  Copyright (c) 2026 tanvir-techbro.                              *
 *  You may opt to use, copy, modify, merge, publish, distribute    *
 *  and/or sell copies of the Software, and permit persons to whom  *
 *  the Software is furnished to do so, under the conditions of the *
 *  LICENSE.                                                        *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, *
 *  EXPRESS OR IMPLIED.                                             *
 *                                                                  *
 *  If you find any bug you would be highly encouraged to create a  *
 *  github issue at <https://github.com/tanvir-techbro/PiNum-Lang>  *
 *  or contact <surjointelligence.team@gmail.com>                   *
 ********************************************************************/

// NOTE: This parser uses recursive decent parsing method.
#include "../include/parser.h"

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
// - Expression parsing -
ASTnode *parse_primary(Parser *parser) {
        if (match(parser, TOKEN_INUM)) {
                return make_int_node(parser->tokens->tokens[parser->current - 1].int_value);
        }
        if (match(parser, TOKEN_FNUM)) {
                return make_float_node(parser->tokens->tokens[parser->current - 1].float_value);
        }
        if (match(parser, TOKEN_ID)) {
                return make_identifier_node(parser->tokens->tokens[parser->current - 1].value);
        }

        if (match(parser, TOKEN_LRPAREN)) {
                ASTnode *expr = parse_expression(parser);
                consume(parser, TOKEN_RRPAREN, "expect ')' after expression.\n");
                return expr;
        }

        // Error
        fprintf(stderr, "\033[1;31msyntax error:\033[0m unexpected token %s\n", peek(parser).value);
        exit(EXIT_FAILURE);
}
ASTnode *parse_factors(Parser *parser) {
        ASTnode *node = parse_primary(parser);

        // TOKEN_STAR may be used for multiplication, TOKEN_FSLASH may be used for division and TOKEN_PERCENT may be used for modulo
        while (match(parser, TOKEN_STAR) || match(parser, TOKEN_FSLASH) || match(parser, TOKEN_PERCENT)) {
                tokenType op = parser->tokens->tokens[parser->current - 1].type;
                ASTnode *right = parse_primary(parser);
                node = make_binary_node(node, op, right);
        }
        return node;
}
ASTnode *parse_term(Parser *parser) {
        ASTnode *node = parse_factors(parser);

        while (match(parser, TOKEN_PLUS) || match(parser, TOKEN_MINUS)) {
                tokenType op = parser->tokens->tokens[parser->current - 1].type;
                ASTnode *right = parse_factors(parser);
                node = make_binary_node(node, op, right);
        }
        return node;
}
// Entry point
ASTnode *parse_expression(Parser *parser) {
        return parse_term(parser);
}
// ----------------------
