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

// NOTE: Hashng algorithm used FNV-1a
#include "../include/_hashmap.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define FNV_OFFSET_BASIS 14695981039346656037ULL // 64 bit
#define FNV_PRIME 1099511628211ULL

// --- built in hash/eq helpers ---
// hm_hash_fn
size_t hm_hash_str(const void *key) {
        const unsigned char *k = key;
        size_t hash = FNV_OFFSET_BASIS;
        while (*k) {
                hash ^= (unsigned char)(*k);
                hash *= FNV_PRIME;
                k++;
        }
        return hash;
}
size_t hm_hash_int(const void *key) {
        uint64_t hash = (size_t)*(const int *)key;
        hash ^= hash >> 33; // simple mixer so ints like 2,4,8,16 don't clump in the same buckets
        hash *= 0xff51afd7ed558ccdULL;
        hash ^= hash >> 33;
        return (size_t)hash;
}
size_t hm_hash_ptr(const void *key) {
        uintptr_t hash = (uintptr_t)key; // pointers are already well spread
        return (size_t)hash ^ (hash >> 32);
}
// hm_eq_fn
bool hm_eq_str(const void *a, const void *b) {
        if (a == b) return true; // Quick check: same pointer address
        if (!a || !b) return false;

        return strcmp((const char *)a, (const char *)b) == 0;
}
bool hm_eq_int(const void *a, const void *b) {
        if (a == b) return true;
        if (!a || !b) return false;

        return *(const int *)a == *(const int *)b;
}
bool hm_eq_ptr(const void *a, const void *b) {
        return a == b;
}
