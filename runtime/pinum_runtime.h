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
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ANSI color codes
#define ANSI_BOLD "\033[1;40m"   // bold program name
#define ANSI_RED "\033[1;31m"    // errors
#define ANSI_YELLOW "\033[1;33m" // warnings
#define ANSI_RESET "\033[0m"

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

/**
 * @brief same shit as __pinum_repeat_char__ but for char* type
 * */
static inline char *__pinum_repeat_string__(const char *str, int count) {
        // return empty string if the given string is empty
        if (!str || count <= 0) {
                char *empty = (char *)malloc(1);
                if (empty) empty[0] = '\0';
                return empty;
        }

        size_t len = strlen(str);
        char *out = (char *)malloc((len * count) + 1);
        if (!out) {
                perror(ANSI_BOLD "pinum: " ANSI_RED "runtime: error: " ANSI_RESET "out of memory");
                exit(EXIT_FAILURE);
        }

        // keep track of the position
        char *ptr = out;
        for (int i = 0; i < count; i++) {
                memcpy(ptr, str, len);
                ptr += len;
        }
        *ptr = '\0';
        return out;
}

/**
 * @brief takes 2 string and adds them
 * @returns combined string
 * */
static inline char *__pinum_add_string__(const char *s1, const char *s2) {
        // Initialize the strings if null
        if (!s1) s1 = "";
        if (!s2) s2 = "";

        size_t len1 = strlen(s1);
        size_t len2 = strlen(s2);
        char *out = (char *)malloc(len1 + len2 + 1); // +1 for null terminator
        if (!out) {
                perror(ANSI_BOLD "pinum: " ANSI_RED "runtime: error: " ANSI_RESET "out of memory");
                exit(EXIT_FAILURE);
        }

        memcpy(out, s1, len1);
        memcpy(out + len1, s2, len2);
        out[len1 + len2] = '\0';

        return out;
}
// ------------

#endif // !PINUM_RUNTIME_H
