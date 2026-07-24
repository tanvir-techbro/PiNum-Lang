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
ASTnode *parse_line(token_list *tokens) {
        Parser parser = {tokens, 0};
        return parse_statement(&parser);
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
// - statement level parsing -
ASTnode *parse_statement(Parser *parser) {
        if (check(parser, TOKEN_INT) || check(parser, TOKEN_FLOAT) ||
            check(parser, TOKEN_CHAR) || check(parser, TOKEN_STRING) ||
            check(parser, TOKEN_BOOL) || check(parser, TOKEN_UNSIGNED) ||
            check(parser, TOKEN_SIGNED)) {
                return parse_declaration(parser);
        }
        if (match(parser, TOKEN_PRINT)) {
                ASTnode *expression = parse_expression(parser);
                consume_end_of_statement(parser);
                ASTnode *node = create_ast_node(NODE_PRINT);
                node->data.print.expression = expression;
                return node;
        }
        if (match(parser, TOKEN_IF)) {
                return parse_if_statement(parser);
        }
        if (match(parser, TOKEN_WHILE)) {
                return parse_while_statement(parser);
        }
        if (match(parser, TOKEN_ATSIGN)) {
                // The lexer splits '@import' into TOKEN_ATSIGN and TOKEN_IMPORT
                token name_token = advance(parser);
                char *name = name_token.value;
                char *value = NULL;
                // Allow IDs or LIB tokens
                if (check(parser, TOKEN_ID) || check(parser, TOKEN_LIB_STDLIB)) {
                        value = advance(parser).value;
                }
                return make_directive_node(name, value);
        }

        // Skip newlines before statement
        if (match(parser, TOKEN_NLINE)) {
                // If it is a newline, we have already consumed it with match.
                // Just return NULL or continue to next statement.
                return NULL;
        }

        // Fallback to expression statement
        ASTnode *expression = parse_expression(parser);
        consume_end_of_statement(parser);
        return expression;

        // TODO: add more statements to be parsed
}
ASTnode *parse_if_statement(Parser *parser) {
        // Parsing the expresseion inside ( )
        consume(parser, TOKEN_LRPAREN, "expected '('\n");
        ASTnode *condition = parse_expression(parser);
        consume(parser, TOKEN_RRPAREN, "expected ')'\n");

        ASTnode *then_block = parse_block(parser);
        ASTnode *else_block = NULL;
        if (match(parser, TOKEN_ELSE)) {
                else_block = parse_block(parser);
        }

        return make_if_stat_node(condition, then_block, else_block);
}
ASTnode *parse_while_statement(Parser *parser) {
        // parsing the expression inside ( )
        consume(parser, TOKEN_LRPAREN, "expected '('\n");
        ASTnode *condition = parse_expression(parser);
        consume(parser, TOKEN_RRPAREN, "expected ')'\n");
        ASTnode *body = parse_block(parser);

        return make_while_node(condition, body);
}
ASTnode *parse_declaration(Parser *parser) {
        char *modifier = NULL;
        char *data_type = NULL;

        // Modifier (optional)
        if (match(parser, TOKEN_UNSIGNED)) modifier = "unsigned";
        else if (match(parser, TOKEN_SIGNED)) modifier = "signed";
        else if (match(parser, TOKEN_LONG)) modifier = "long";
        else if (match(parser, TOKEN_SHORT)) modifier = "short";

        // Data Type (Required)
        if (match(parser, TOKEN_INT)) data_type = "int";
        else if (match(parser, TOKEN_FLOAT)) data_type = "float";
        else if (match(parser, TOKEN_DOUBLE)) data_type = "double";
        else if (match(parser, TOKEN_CHAR)) data_type = "char";
        else if (match(parser, TOKEN_STRING)) data_type = "string";
        else if (match(parser, TOKEN_BOOL)) data_type = "bool";
        else {
                fprintf(stderr, "Syntax error: Expected type name.\n");
                exit(EXIT_FAILURE);
        }

        // Veriable name
        token name_token = consume(parser, TOKEN_ID, "expected veriable name.\n");
        char *var_name = name_token.value; // value is actually the name of the token

        ASTnode *initializer = NULL;
        if (match(parser, TOKEN_EQUAL)) {
                initializer = parse_expression(parser);
        }
        // expecting for a semicolon at the end
        consume_end_of_statement(parser);

        return make_var_decl_node(data_type, modifier, var_name, initializer, false, 0);
}
ASTnode *parse_block(Parser *parser) {
        consume(parser, TOKEN_LCPAREN, "expected '{'\n");
        ASTnode *block = create_ast_node(NODE_BLOCK);

        while (!check(parser, TOKEN_RCPAREN) && !check(parser, TOKEN_EOF)) {
                if (match(parser, TOKEN_NLINE)) continue;
                ast_add_statement(block, parse_statement(parser));
        }

        consume(parser, TOKEN_RCPAREN, "expected '}'\n");
        return block;
}
// - Expression parsing -
// Takes the parsed binary node and makes a specific node
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

        if (match(parser, TOKEN_DQUOTE) || match(parser, TOKEN_SQUOTE)) {
                // Collect string content
                char *str_content = strdup("");
                while (!check(parser, TOKEN_DQUOTE) && !check(parser, TOKEN_SQUOTE) && !check(parser, TOKEN_EOF)) {
                        token t = advance(parser);
                        if (t.value) {
                                char *new_str = malloc(strlen(str_content) + strlen(t.value) + 1);
                                sprintf(new_str, "%s%s", str_content, t.value);
                                free(str_content);
                                str_content = new_str;
                        }
                }
                advance(parser); // consume closing quote
                ASTnode *node = make_string_node(str_content);
                free(str_content);
                return node;
        }

        // Error
        fprintf(stderr, "\033[1;31msyntax error:\033[0m unexpected token %s\n", peek(parser).value);
        exit(EXIT_FAILURE);
}
// parsing * (multiplication), / (division) and % (modulo)
// parses and refers to parse_primary
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
// parsing + (addidtion) and - (subtraction)
// parses and refers to parse_factors
ASTnode *parse_term(Parser *parser) {
        ASTnode *node = parse_factors(parser);

        while (match(parser, TOKEN_PLUS) || match(parser, TOKEN_MINUS)) {
                tokenType op = parser->tokens->tokens[parser->current - 1].type;
                ASTnode *right = parse_factors(parser);
                node = make_binary_node(node, op, right);
        }
        return node;
}
// parsing < (lesser than), > (greater than), <= (less than equal to) and >= (more than equal to)
// parses and refers to parse_term
ASTnode *parse_comparison(Parser *parser) {
        ASTnode *node = parse_term(parser);

        while (match(parser, TOKEN_LABRACKET) || match(parser, TOKEN_RABRACKET) ||
               match(parser, TOKEN_LEQUAL) || match(parser, TOKEN_GEQUAL)) {
                tokenType op = parser->tokens->tokens[parser->current - 1].type;
                ASTnode *right = parse_term(parser);
                node = make_binary_node(node, op, right);
        }
        return node;
}
// parsing == (equal to) and != (not equal to)
// parses and refers to parse_comparison
ASTnode *parse_equality(Parser *parser) {
        ASTnode *node = parse_comparison(parser);

        while (match(parser, TOKEN_EEQUAL) || match(parser, TOKEN_NEQUAL)) {
                tokenType op = parser->tokens->tokens[parser->current - 1].type;
                ASTnode *right = parse_comparison(parser);
                node = make_binary_node(node, op, right);
        }
        return node;
}
// parsing && (and operator)
// refers to parse_equality
ASTnode *parse_logical_and(Parser *parser) {
        ASTnode *node = parse_equality(parser);

        while (match(parser, TOKEN_AND)) {
                tokenType op = parser->tokens->tokens[parser->current - 1].type;
                ASTnode *right = parse_equality(parser);
                node = make_binary_node(node, op, right);
        }
        return node;
}
// parsing || (or operator)
// refers to parse_logical_and
ASTnode *parse_logical_or(Parser *parser) {
        ASTnode *node = parse_logical_and(parser);

        while (match(parser, TOKEN_OR)) {
                tokenType op = parser->tokens->tokens[parser->current - 1].type;
                ASTnode *right = parse_logical_and(parser);
                node = make_binary_node(node, op, right);
        }
        return node;
}
// parsing assignition
ASTnode *parse_assignment(Parser *parser) {
        ASTnode *node = parse_logical_or(parser);

        if (match(parser, TOKEN_EQUAL)) {
                ASTnode *value = parse_assignment(parser);
                if (node->type != NODE_IDENTIFIER) {
                        fprintf(stderr, "Syntax error: Invalid assignment target.\n");
                        exit(EXIT_FAILURE);
                }
                return make_assign_node(node->data.identifier.name, value);
        }
        return node;
}
// Entry point
// refers to parse_assignment
ASTnode *parse_expression(Parser *parser) {
        return parse_assignment(parser);
}
