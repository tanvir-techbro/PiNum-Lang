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

// ~./pinum-lang/runtime/pinum_runtime.h
#ifndef PINUM_RUNTIME_H
#define PINUM_RUNTIME_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- HELPER ---
// --------------

// --- MAIN ---
/**
 * @brief takes binary operations done with char and return a char* type
 * @param c takes the char input
 * @param count is the number of time c is repeated
 * @example `char c = ' '; print(c * 5)` will repeat c five times
 * */
static inline char *__pinum_repeat_char__(char c, int count) {
        char *out = (char *)malloc((size_t)count + 1);
        for (int i = 0; i < count; i++) {
                out[i] = c;
        }
        // add null terminator add the end
        out[count] = '\0';
        return out;
}
// ------------

#endif // !PINUM_RUNTIME_H
