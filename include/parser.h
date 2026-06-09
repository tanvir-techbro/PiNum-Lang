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
