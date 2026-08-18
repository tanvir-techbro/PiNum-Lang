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

#include "../include/methods.h"
#include <string.h>

const method_def METHODS[] = {
    {"vec_", "append", "__pinum_%s_append", NULL},
};
const int METHOD_COUNT = sizeof(METHODS) / sizeof(METHODS[0]);

// NOTE: hashmap will be implimented when the METHODS exceed 32 elements
const method_def *method_lookup(const char *obj_type, const char *name) {
        for (int i = 0; i < METHOD_COUNT; i++) {
                if (strncmp(obj_type, METHODS[i].type_prefix, strlen(METHODS[i].type_prefix)) == 0 &&
                    strcmp(name, METHODS[i].name) == 0) {
                        return &METHODS[i];
                }
        }
        return NULL;
}
