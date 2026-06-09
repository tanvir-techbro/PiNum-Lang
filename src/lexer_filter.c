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

// NOTE: Different filter functions in include/lexer.h will be defined here.
// These functions are used to filter unusual tokens and refine tokens.

#include "../include/lexer.h"
#include "../include/mode.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Keeps coments from being listed in the token list.
// Whatever is after a hashtag outside print or any wtring function, will be treated as a comment.
// We only support 1 line comment.
token token_ignore_comment(token t, FILE *buffer) {
        while (t.type != TOKEN_NLINE && t.type != TOKEN_EOF) {
                DQUOTE_MODE = false;
                if (t.value) {
                        free(t.value);
                }
                t = lexer_tokenizer(buffer);
        }
        return t;
}
