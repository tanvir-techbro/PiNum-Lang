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

#include "../include/sema.h"
#include "../include/error.h"
#include <stdlib.h>
#include <string.h>

// --- HELPERS ---
// - scope stack -
static void sem_push_scope(SemAnalyzer *a) {
        // dynamic sizing
        if (a->frame_count >= a->frame_capacity) {
                a->frame_capacity = a->frame_capacity ? a->frame_capacity * 2 : 8;
                HashMap **tmp = realloc(a->frames, a->frame_capacity * sizeof(HashMap *));
                if (!tmp) {
                        pinum_error(STAGE_INTERNAL, ERR_ALLOC_FAILED, NULL);
                }
                a->frames = tmp;
        }
        a->frames[a->frame_count++] = hashmap_create(hm_hash_str, hm_eq_str, free, free);
}
static void sem_pop_scope(SemAnalyzer *a) {
        if (a->frame_count == 0) {
                return; // for stack underflow
        }
        hashmap_free(a->frames[--a->frame_count]);
}
// - declare / resolve -
static void sem_declare(SemAnalyzer *a, const char *name, const char *type) {
        // previous scope / frame
        HashMap *frame = a->frames[a->frame_count - 1];
        char *k = strdup(name);
        char *v = strdup(type);
        // returns false if key exists
        if (!hashmap_insert(frame, k, v)) {
                free(k);
                free(v);
                pinum_error_at(STAGE_SEMANTIC, ERR_REDECLARED_VAR, 0, 0, name);
        }
}
static const char *sem_resolve(SemAnalyzer *a, const char *name) {
        // iterate from the top scope to see if the veriable exists
        for (int i = (int)a->frame_count - 1; i >= 0; i--) {
                bool found;
                const char *type = hashmap_get(a->frames[i], name, &found);
                if (found) {
                        return type;
                }
        }
        return NULL;
}
// full C type for a symbol, e.g. "long int", "char *"
static char *sem_fulltype(const char *type_name, const char *modifiers) {
        const char *base = strcmp(type_name, "string") == 0 ? "char *" : type_name;
        if (modifiers == NULL) {
                return strdup(base);
        }
        char *out = malloc(strlen(modifiers) + strlen(base) + 2);
        sprintf(out, "%s %s", modifiers, base);
        return out;
}

// --- MAIN ---
void semantic_analyze(ASTnode *program) {
        SemAnalyzer a = {0};
        sem_push_scope(&a);
        sem_pop_scope(&a);
        free(a.frames);
}
