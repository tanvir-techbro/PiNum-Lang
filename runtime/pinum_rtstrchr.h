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
 *  github issue at <https://github.com/pinum-project/PiNum-Lang>   *
 *  or contact <surjointelligence.team@gmail.com>                   *
 ********************************************************************/

#ifndef PINUM_RTSTRCHR_H
#define PINUM_RTSTRCHR_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// 'c' repeated `count` times → a heap string ("   " for (' ', 3))
static inline char *__pinum_repeat_char(char c, int count) {
        if (!c || count <= 0) {
                char *empty = (char *)malloc(1);
                if (empty) {
                        empty[0] = '\0';
                }
                return empty;
        }
        char *out = (char *)malloc((size_t)count + 1);
        if (!out) {
                return NULL;
        }
        for (int i = 0; i < count; i++) {
                out[i] = c;
        }
        out[count] = '\0';
        return out;
}

// `str` repeated `count` times → a heap string
static inline char *__pinum_repeat_string(const char *str, int count) {
        if (!str || count <= 0) {
                char *empty = (char *)malloc(1);
                if (empty) {
                        empty[0] = '\0';
                }
                return empty;
        }
        size_t len = strlen(str);
        char *out = (char *)malloc((len * (size_t)count) + 1);
        if (!out) {
                return NULL;
        }
        char *ptr = out;
        for (int i = 0; i < count; i++) {
                memcpy(ptr, str, len);
                ptr += len;
        }
        *ptr = '\0';
        return out;
}

// concatenates two strings → a heap string
static inline char *__pinum_add_string(const char *s1, const char *s2) {
        if (!s1) s1 = "";
        if (!s2) s2 = "";

        size_t len1 = strlen(s1);
        size_t len2 = strlen(s2);
        char *out = (char *)malloc(len1 + len2 + 1); // +1 for null terminator
        if (!out) {
                return NULL;
        }
        memcpy(out, s1, len1);
        memcpy(out + len1, s2, len2);
        out[len1 + len2] = '\0';
        return out;
}

#endif // !PINUM_RTSTRCHR_H
