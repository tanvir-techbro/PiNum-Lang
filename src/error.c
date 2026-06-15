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

#include "../include/error.h"

void pinum_error(ErrorCode code, const char *detail) {
        fprintf(stderr, "\033[1;40mpinum:\033[0m 033[1;31merror:\033[0m ");

        switch (code) {
        case ERR_UNEXPECTED_TOKEN:
                fprintf(stderr, "unexpected token '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_EXPECTED_TYPE:
                fprintf(stderr, "expected data type (int, float, etc.) but found '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_EXPECTED_ID:
                fprintf(stderr, "expected identifier but found '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_MISSING_SEMICOLON:
                fprintf(stderr, "missing ';' after statement. Found '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_MISSING_PAREN:
                fprintf(stderr, "missing closing parenthesis. Found '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_INVALID_MODIFIER:
                fprintf(stderr, "invalid modifier '%s'\n", detail ? detail : "unknown");
                break;
        default:
                fprintf(stderr, "an unknown error occurred.\n");
                break;
        }

        exit(EXIT_FAILURE);
}
