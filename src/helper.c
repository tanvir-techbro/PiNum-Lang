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

// NOTE: This file holds all the helper functions that helps other processes run.

#include "../include/lexer.h"
#include "../include/mode.h"
#include "../include/parser.h"
#include <stdbool.h>
#include <string.h>

// code generation mode
bool ENGINE_MODE = false;

// NOTE: The below function is from include/mode.h
bool check_program_mode(token_list *list) {
        // The size will be 4 because the newline character is included in the list
        // The list would be something like ["@", "for", "engine", '\n']
        if (!(list->size == 4 && list->tokens[0].type == TOKEN_ATSIGN)) {
                return false; // return engine mode which is false
        }
        if (!(list->tokens[1].type == TOKEN_ID && list->tokens[2].type == TOKEN_ID)) {
                return false; // return engine mode which is false
        }
        if (strcmp("for", list->tokens[1].value) == 0 && strcmp("engine", list->tokens[2].value) == 0) {
                ENGINE_MODE = true;
        }

        return ENGINE_MODE;
}
// ===============================================

// NOTE: The below functions are from include/lexer.h
void token_list_init(token_list *list) {
        list->size = 0;
        list->capacity = 16;
        list->tokens = malloc(list->capacity * sizeof(token));
}
void token_list_add(token_list *list, token t) {
        if (list->size >= list->capacity) {
                list->capacity *= 2;
                list->tokens = realloc(list->tokens, list->capacity * sizeof(token));
        }
        list->tokens[list->size++] = t;
}
void token_list_free(token_list *list) {
        for (size_t i = 0; i < list->size; i++) {
                if (list->tokens[i].value) {
                        free(list->tokens[i].value);
                        list->tokens[i].value = NULL;
                }
        }
        free(list->tokens);
        list->tokens = NULL;
        list->size = 0;
        list->capacity = 0;
}
// ==================================================

// NOTE: The below functions are from include/parser.h
// Peeks/takes a looks at the current token without moving to the next token.
token peek(Parser *parser) {
        if (parser->current >= parser->tokens->size) {
                return parser->tokens->tokens[parser->tokens->size - 1];
        }
        return parser->tokens->tokens[parser->current];
}
// moves to the next token
token advance(Parser *parser) {
        if (parser->current < parser->tokens->size) {
                parser->current++;
        }
        return parser->tokens->tokens[parser->current - 1];
}
// checks the current token type
bool check(Parser *parser, tokenType type) {
        return peek(parser).type == type;
}
// checks if current token matches with another token and moves on
bool match(Parser *parser, tokenType type) {
        if (check(parser, type)) {
                advance(parser);
                return true;
        }
        // return false if it dosent match and stays at the same place (dosent move on)
        return false;
}
// checks for error and throws error messages
token consume(Parser *parser, tokenType type, const char *message) {
        if (check(parser, type)) {
                return advance(parser);
        }
        fprintf(stderr, "Parser error: %s", message);
        exit(EXIT_FAILURE);
}
void consume_end_of_statement(Parser *parser) {
        if (match(parser, TOKEN_SEMICOLON)) {
                return;
        }
        // If the next line is EOF its the end of statement, check for EOF
        if (check(parser, TOKEN_EOF)) {
                return;
        }

        fprintf(stderr, "Parser error: Expect ';' after statement. Found: %s\n", peek(parser).value);
        exit(EXIT_FAILURE);
}
// ===================================================
