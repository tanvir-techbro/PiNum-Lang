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
 *  github issue at <https://github.com/pinum-project/PiNum-Lang>  *
 *  or contact <surjointelligence.team@gmail.com>                   *
 ********************************************************************/

// NOTE: Hashng algorithm used FNV-1a
#include "../include/_hashmap.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FNV_OFFSET_BASIS 14695981039346656037ULL // 64 bit
#define FNV_PRIME 1099511628211ULL

// --- Construction and Destruction ---
HashMap *hashmap_create(hm_hash_fn hash, hm_eq_fn eq, hm_free_fn key_free, hm_free_fn value_free) {
        HashMap *map = malloc(sizeof(HashMap));
        if (map == NULL) {
                return NULL;
        }
        const size_t initial_capacity = 16;
        map->buckets = calloc(initial_capacity, sizeof(HMNode *));
        if (map->buckets == NULL) {
                free(map);
                return NULL;
        }
        map->capacity = initial_capacity;
        map->size = 0;
        map->max_load_factor = 1.0f;
        map->hash = hash;
        map->eq = eq;
        map->key_free = key_free;     // NULL, caller owns keys
        map->value_free = value_free; // NULL, caller owns values
        return map;
}
void hashmap_free(HashMap *map) {
        if (map == NULL) {
                return;
        }
        hashmap_clear(map);
        free(map->buckets);
        free(map);
}
void hashmap_clear(HashMap *map) {
        for (size_t i = 0; i < map->capacity; i++) {
                HMNode *n = map->buckets[i];
                // traverse linked list
                while (n) {
                        HMNode *next = n->next;
                        if (map->key_free) map->key_free(n->key);
                        if (map->value_free) map->value_free(n->value);
                        free(n);
                        n = next;
                }
                map->buckets[i] = NULL;
        }
        map->size = 0;
}

// --- capacity ---
size_t hashmap_size(HashMap *map) {
        return map->size;
}
bool hashmap_empty(HashMap *map) {
        if (map->size == 0) {
                return true;
        }
        return false;
}

// --- element access / lookup ---
bool hashmap_insert(HashMap *map, void *key, void *value) {
        size_t hash = map->hash(key);
        size_t idx = hash & (map->capacity - 1); // power-of-two trick

        // walk the chain with eq_fn
        // check if the key already exists
        for (HMNode *n = map->buckets[idx]; n; n = n->next) {
                if (map->eq(key, n->key)) {
                        return false;
                }
        }
        // create new storage slot
        HMNode *node = malloc(sizeof(HMNode));
        if (node == NULL) {
                return false;
        }
        node->key = key;
        node->value = value;
        node->hash = hash;
        node->next = map->buckets[idx];
        map->buckets[idx] = node;
        map->size++;
        return true;
}
bool hashmap_put(HashMap *map, void *key, void *value) {
        size_t hash = map->hash(key);
        size_t idx = hash & (map->capacity - 1);

        for (HMNode *n = map->buckets[idx]; n; n = n->next) {
                if (map->eq(key, n->key)) {
                        if (map->value_free) map->value_free(n->value);
                        n->value = value; // overwrite, size unchanged
                        return true;
                }
        }
        // not found → insert new node (same as insert)
        HMNode *node = malloc(sizeof(HMNode));
        if (node == NULL) return false;
        node->key = key;
        node->value = value;
        node->hash = hash;
        node->next = map->buckets[idx];
        map->buckets[idx] = node;
        map->size++;
        return true;
}
void *hashmap_get(HashMap *map, const void *key, bool *found) {
        size_t hash = map->hash(key);
        size_t idx = hash & (map->capacity - 1); // power-of-two trick

        for (HMNode *n = map->buckets[idx]; n; n = n->next) {
                if (map->eq(key, n->key)) {
                        *found = true;
                        return n->value;
                }
        }
        *found = false;
        return NULL;
}
bool hashmap_contains(HashMap *map, const void *key) {
        //
}

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
        uintptr_t hash = (uintptr_t)key;
        hash ^= hash >> 16; // works on both 32-bit and 64-bit
        hash ^= hash >> 8;
        return (size_t)hash;
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
