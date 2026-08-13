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
#include <stdlib.h>
#include <string.h>

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
            check(parser, TOKEN_DOUBLE) || check(parser, TOKEN_CHAR) ||
            check(parser, TOKEN_STRING) || check(parser, TOKEN_BOOL) ||
            check(parser, TOKEN_UNSIGNED) || check(parser, TOKEN_SIGNED) ||
            check(parser, TOKEN_LONG) || check(parser, TOKEN_SHORT)) {
                return parse_declaration(parser);
        }
        if (match(parser, TOKEN_PRINT)) {
                consume(parser, TOKEN_LRPAREN, "'(' after print");
                ASTnode *node = make_print_node();
                if (!check(parser, TOKEN_RRPAREN)) {
                        do {
                                ast_add_print_arg(node, parse_expression(parser));
                        } while (match(parser, TOKEN_COMMA));
                }
                consume(parser, TOKEN_RRPAREN, "')' after arguments");
                consume_end_of_statement(parser);
                return node;
        }
        if (match(parser, TOKEN_READ)) {
                return parse_read_statement(parser);
        }
        if (match(parser, TOKEN_IF)) {
                return parse_if_statement(parser);
        }
        if (match(parser, TOKEN_WHILE)) {
                return parse_while_statement(parser);
        }
        if (match(parser, TOKEN_FOR)) {
                return parse_for_statement(parser);
        }
        if (match(parser, TOKEN_RETURN)) {
                return parse_return_statement(parser);
        }
        if (match(parser, TOKEN_BREAK)) {
                consume_end_of_statement(parser);
                return create_ast_node(NODE_BREAK);
        }
        if (match(parser, TOKEN_CONTINUE)) {
                consume_end_of_statement(parser);
                return create_ast_node(NODE_CONTINUE);
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
        consume(parser, TOKEN_LRPAREN, "'('");
        ASTnode *condition = parse_expression(parser);
        consume(parser, TOKEN_RRPAREN, "')'");

        ASTnode *then_block = parse_block(parser);
        ASTnode *else_block = NULL;
        if (match(parser, TOKEN_ELSE)) {
                // check for 'else if'
                if (match(parser, TOKEN_IF)) {
                        else_block = parse_if_statement(parser);
                } else {
                        else_block = parse_block(parser);
                }
        }

        return make_if_stat_node(condition, then_block, else_block);
}
ASTnode *parse_while_statement(Parser *parser) {
        // parsing the expression inside ( )
        consume(parser, TOKEN_LRPAREN, "'('");
        ASTnode *condition = parse_expression(parser);
        consume(parser, TOKEN_RRPAREN, "')'");
        ASTnode *body = parse_block(parser);

        return make_while_node(condition, body);
}
// for(init; condition; inc) {body}
ASTnode *parse_for_statement(Parser *parser) {
        consume(parser, TOKEN_LRPAREN, "'('");

        // init
        ASTnode *init = NULL;
        if (!check(parser, TOKEN_SEMICOLON)) {
                if (check(parser, TOKEN_INT) || check(parser, TOKEN_FLOAT) ||
                    check(parser, TOKEN_DOUBLE)) {
                        init = parse_declaration(parser);
                } else {
                        init = parse_expression(parser);
                        consume(parser, TOKEN_SEMICOLON, "';'");
                }
        } else {
                advance(parser); // skip empty init ';'
        }
        // condition
        ASTnode *condition = NULL;
        if (!check(parser, TOKEN_SEMICOLON)) {
                condition = parse_expression(parser);
        }
        consume(parser, TOKEN_SEMICOLON, "';'");
        // inc
        ASTnode *inc = NULL;
        if (!check(parser, TOKEN_RRPAREN)) {
                inc = parse_expression(parser);
        }
        consume(parser, TOKEN_RRPAREN, "')'");

        ASTnode *body = parse_block(parser);
        return make_for_node(init, condition, inc, body);
}
ASTnode *parse_return_statement(Parser *parser) {
        ASTnode *expression = parse_expression(parser);
        consume_end_of_statement(parser);
        ASTnode *node = create_ast_node(NODE_RETURN);
        node->data.returns.expression = expression;
        return node;
}
ASTnode *parse_read_statement(Parser *parser) {
        consume(parser, TOKEN_LRPAREN, "'(' after read");
        token name_token = consume(parser, TOKEN_ID, "a variable name");
        consume(parser, TOKEN_RRPAREN, "')' after variable name");
        consume_end_of_statement(parser);
        return make_read_node(name_token.value);
}
// verifies veriable modifier
static bool is_valid_modifier(const char *modifier, const char *data_type) {
        if (modifier == NULL) return true;
        if (strcmp(modifier, "unsigned") == 0 || strcmp(modifier, "signed") == 0)
                return strcmp(data_type, "int") == 0 || strcmp(data_type, "char") == 0;
        if (strcmp(modifier, "long") == 0)
                return strcmp(data_type, "int") == 0 || strcmp(data_type, "double") == 0;
        if (strcmp(modifier, "short") == 0)
                return strcmp(data_type, "int") == 0;
        return false;
}
ASTnode *parse_declaration(Parser *parser) {
        char *modifier = NULL;
        char *data_type = NULL;
        token modifier_token = {0};

        // Modifier (optional)
        if (match(parser, TOKEN_UNSIGNED)) {
                modifier = "unsigned";
                modifier_token = parser->tokens->tokens[parser->current - 1];
        } else if (match(parser, TOKEN_SIGNED)) {
                modifier = "signed";
                modifier_token = parser->tokens->tokens[parser->current - 1];
        } else if (match(parser, TOKEN_LONG)) {
                modifier = "long";
                modifier_token = parser->tokens->tokens[parser->current - 1];
        } else if (match(parser, TOKEN_SHORT)) {
                modifier = "short";
                modifier_token = parser->tokens->tokens[parser->current - 1];
        }

        // Data Type (Required)
        if (match(parser, TOKEN_INT)) data_type = "int";
        else if (match(parser, TOKEN_FLOAT)) data_type = "float";
        else if (match(parser, TOKEN_DOUBLE)) data_type = "double";
        else if (match(parser, TOKEN_CHAR)) data_type = "char";
        else if (match(parser, TOKEN_STRING)) data_type = "string";
        else if (match(parser, TOKEN_BOOL)) data_type = "bool";
        else {
                token found = peek(parser);
                pinum_expected_at(STAGE_PARSER, found.line, found.col, "a data type (int, float, etc.)", peek_display(parser));
        }

        if (!is_valid_modifier(modifier, data_type)) {
                pinum_error_at(STAGE_PARSER, ERR_INVALID_MODIFIER, modifier_token.line, modifier_token.col, modifier);
        }

        // Veriable name
        token name_token = consume(parser, TOKEN_ID, "a variable name");
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
        consume(parser, TOKEN_LCPAREN, "'{'");
        ASTnode *block = create_ast_node(NODE_BLOCK);

        while (!check(parser, TOKEN_RCPAREN) && !check(parser, TOKEN_EOF)) {
                if (match(parser, TOKEN_NLINE)) continue;
                ast_add_statement(block, parse_statement(parser));
        }

        consume(parser, TOKEN_RCPAREN, "'}'");
        return block;
}

// - Function parsing -
// TODO: implement this
/*
ASTnode *parse_func_def(Parser *parser) {
}
ASTnode *parse_func_def_param(Parser *parser) {
        // return make_var_decl_node(char *type_name, char *modifiers, char *name, ASTnode *value, bool is_array, int array_size);
}
*/

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
        if (match(parser, TOKEN_TRUE)) {
                return make_bool_node(true);
        }
        if (match(parser, TOKEN_FALSE)) {
                return make_bool_node(false);
        }

        if (match(parser, TOKEN_LRPAREN)) {
                ASTnode *expr = parse_expression(parser);
                consume(parser, TOKEN_RRPAREN, "')' after expression");
                return expr;
        }

        if (match(parser, TOKEN_DQUOTE) || match(parser, TOKEN_SQUOTE)) {
                tokenType quote = parser->tokens->tokens[parser->current - 1].type;
                // Collect string content
                char *str_content = strdup("");
                int content_tokens = 0;
                while (!check(parser, TOKEN_DQUOTE) && !check(parser, TOKEN_SQUOTE) && !check(parser, TOKEN_EOF)) {
                        token t = advance(parser);
                        content_tokens++;
                        if (t.value) {
                                char *new_str = malloc(strlen(str_content) + strlen(t.value) + 1);
                                sprintf(new_str, "%s%s", str_content, t.value);
                                free(str_content);
                                str_content = new_str;
                        }
                }
                advance(parser); // consume closing quote
                // a single-quoted literal with exactly one 1-character token is a char literal
                if (quote == TOKEN_SQUOTE && content_tokens == 1 && str_content[0] != '\0' && str_content[1] == '\0') {
                        char c = str_content[0];
                        free(str_content);
                        return make_char_node(c);
                }
                ASTnode *node = make_string_node(str_content);
                free(str_content);
                return node;
        }

        // Error
        token found = peek(parser);
        pinum_error_at(STAGE_PARSER, ERR_UNEXPECTED_TOKEN, found.line, found.col, peek_display(parser));
}
// parses function call and member access
// parses and refers to parse_primary
ASTnode *parse_call(Parser *parser) {
        ASTnode *node = parse_primary(parser);

        while (true) {
                // function call parsing
                // function call identified by NODE_IDENTIFIER followed by TOKEN_LRPAREN
                if (match(parser, TOKEN_LRPAREN)) {
                        if (node->type != NODE_IDENTIFIER) {
                                token trigger = parser->tokens->tokens[parser->current - 1];
                                pinum_error_at(STAGE_PARSER, ERR_INVALID_CALL_TARGET, trigger.line, trigger.col, node_type_name(node->type));
                        }
                        char *name = strdup(node->data.identifier.name);
                        free_ast_node(node);
                        ASTnode *call = make_func_call_node(name, NULL, 0);
                        free(name);
                        // if the next token is not ')' TOKEN_RRPAREN, there must be args
                        if (!check(parser, TOKEN_RRPAREN)) {
                                do {
                                        ast_add_arg(call, parse_expression(parser));
                                } while (match(parser, TOKEN_COMMA));
                        }
                        consume(parser, TOKEN_RRPAREN, "')' after arguments");
                        node = call;
                }
                // array access
                // array access if NODE_IDENTIFIER followed by TOKEN_LSPAREN
                else if (match(parser, TOKEN_LSPAREN)) {
                        ASTnode *index = parse_expression(parser);
                        consume(parser, TOKEN_RSPAREN, "']' after index");
                        ASTnode *access = create_ast_node(NODE_ARRAY_ACCESS);
                        access->data.array_access.name = node->data.identifier.name;
                        access->data.array_access.index = index;
                        node = access;
                } else {
                        break;
                }
        }
        return node;
}
// parsing ! and - in front of values
// parses and refers to parse_call
ASTnode *parse_unary(Parser *parser) {
        if (match(parser, TOKEN_EXCLAMATION) || match(parser, TOKEN_MINUS)) {
                tokenType op = parser->tokens->tokens[parser->current - 1].type;
                ASTnode *left = parse_unary(parser);
                return make_unary_node(op, left);
        }
        return parse_call(parser);
}
// parsing * (multiplication), / (division) and % (modulo)
// parses and refers to parse_unary
ASTnode *parse_factors(Parser *parser) {
        ASTnode *node = parse_unary(parser);

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
// parses ternary operations
// parses and calls parse_logical_or
ASTnode *parse_ternary(Parser *parser) {
        ASTnode *node = parse_logical_or(parser);

        ASTnode *then_expr = NULL;
        ASTnode *else_expr = NULL;
        if (match(parser, TOKEN_QUESTION)) {
                then_expr = parse_ternary(parser);
                consume(parser, TOKEN_COLON, "':'");
                else_expr = parse_ternary(parser);
                node = make_ternary_node(node, then_expr, else_expr);
        }
        return node;
}
// parsing assignition
// parses and calls parse_ternary
ASTnode *parse_assignment(Parser *parser) {
        ASTnode *node = parse_ternary(parser);

        if (match(parser, TOKEN_EQUAL)) {
                token trigger = parser->tokens->tokens[parser->current - 1];
                ASTnode *value = parse_assignment(parser);
                if (node->type != NODE_IDENTIFIER) {
                        pinum_error_at(STAGE_PARSER, ERR_INVALID_ASSIGN_TARGET, trigger.line, trigger.col, node_type_name(node->type));
                }
                char *name = strdup(node->data.identifier.name);
                free_ast_node(node);
                ASTnode *assign = make_assign_node(name, value);
                free(name);
                return assign;
        }
        return node;
}
// Entry point
// refers to parse_assignment
ASTnode *parse_expression(Parser *parser) {
        return parse_assignment(parser);
}
