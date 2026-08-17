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

#ifndef PINUM_RTVEC_H
#define PINUM_RTVEC_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* ---------- INT ---------- */
typedef struct {
        int *data;
        size_t size;
        size_t capacity;
} vec_int;

static inline vec_int __pinum_vec_int_init(int count, ...) {
        vec_int v = {NULL, 0, 0};
        if (count > 0) {
                v.data = (int *)malloc(sizeof(int) * (size_t)count);
                if (v.data) {
                        v.size = v.capacity = (size_t)count;
                        va_list ap;
                        va_start(ap, count);
                        for (int i = 0; i < count; i++) {
                                v.data[i] = va_arg(ap, int);
                        }
                        va_end(ap);
                }
        }
        return v;
}
static inline void __pinum_vec_int_append(vec_int *v, int item) {
        if (v->size >= v->capacity) {
                size_t new_cap = v->capacity == 0 ? 4 : v->capacity * 2;
                int *new_data = (int *)realloc(v->data, sizeof(int) * new_cap);
                if (!new_data) {
                        return;
                }
                v->data = new_data;
                v->capacity = new_cap;
        }
        v->data[v->size++] = item;
}
static inline void __pinum_vec_int_print(vec_int v) {
        printf("[");
        for (size_t i = 0; i < v.size; i++) {
                if (i) printf(", ");
                printf("%d", v.data[i]);
        }
        printf("]");
}

/* ---------- FLOAT ---------- */
typedef struct {
        float *data;
        size_t size;
        size_t capacity;
} vec_float;

static inline vec_float __pinum_vec_float_init(int count, ...) {
        vec_float v = {NULL, 0, 0};
        if (count > 0) {
                v.data = (float *)malloc(sizeof(float) * (size_t)count);
                if (v.data) {
                        v.size = v.capacity = (size_t)count;
                        va_list ap;
                        va_start(ap, count);
                        for (int i = 0; i < count; i++) {
                                v.data[i] = va_arg(ap, double);
                        }
                        va_end(ap);
                }
        }
        return v;
}
static inline void __pinum_vec_float_append(vec_float *v, float item) {
        if (v->size >= v->capacity) {
                size_t new_cap = v->capacity == 0 ? 4 : v->capacity * 2;
                float *new_data = (float *)realloc(v->data, sizeof(float) * new_cap);
                if (!new_data) {
                        return;
                }
                v->data = new_data;
                v->capacity = new_cap;
        }
        v->data[v->size++] = item;
}
static inline void __pinum_vec_float_print(vec_float v) {
        printf("[");
        for (size_t i = 0; i < v.size; i++) {
                if (i) printf(", ");
                printf("%f", v.data[i]);
        }
        printf("]");
}

/* ---------- DOUBLE --------- */
typedef struct {
        double *data;
        size_t size;
        size_t capacity;
} vec_double;

static inline vec_double __pinum_vec_double_init(int count, ...) {
        vec_double v = {NULL, 0, 0};
        if (count > 0) {
                v.data = (double *)malloc(sizeof(double) * (size_t)count);
                if (v.data) {
                        v.size = v.capacity = (size_t)count;
                        va_list ap;
                        va_start(ap, count);
                        for (int i = 0; i < count; i++) {
                                v.data[i] = va_arg(ap, double);
                        }
                        va_end(ap);
                }
        }
        return v;
}
static inline void __pinum_vec_double_append(vec_double *v, double item) {
        if (v->size >= v->capacity) {
                size_t new_cap = v->capacity == 0 ? 4 : v->capacity * 2;
                double *new_data = (double *)realloc(v->data, sizeof(double) * new_cap);
                if (!new_data) {
                        return;
                }
                v->data = new_data;
                v->capacity = new_cap;
        }
        v->data[v->size++] = item;
}
static inline void __pinum_vec_double_print(vec_double v) {
        printf("[");
        for (size_t i = 0; i < v.size; i++) {
                if (i) printf(", ");
                printf("%lf", v.data[i]);
        }
        printf("]");
}

/* ---------- CHAR ---------- */
typedef struct {
        char *data;
        size_t size;
        size_t capacity;
} vec_char;

static inline vec_char __pinum_vec_char_init(int count, ...) {
        vec_char v = {NULL, 0, 0};
        if (count > 0) {
                v.data = (char *)malloc(sizeof(char) * (size_t)count);
                if (v.data) {
                        v.size = v.capacity = (size_t)count;
                        va_list ap;
                        va_start(ap, count);
                        for (int i = 0; i < count; i++) {
                                v.data[i] = va_arg(ap, int);
                        }
                        va_end(ap);
                }
        }
        return v;
}
static inline void __pinum_vec_char_append(vec_char *v, char item) {
        if (v->size >= v->capacity) {
                size_t new_cap = v->capacity == 0 ? 4 : v->capacity * 2;
                char *new_data = (char *)realloc(v->data, sizeof(char) * new_cap);
                if (!new_data) {
                        return;
                }
                v->data = new_data;
                v->capacity = new_cap;
        }
        v->data[v->size++] = item;
}
static inline void __pinum_vec_char_print(vec_char v) {
        printf("[");
        for (size_t i = 0; i < v.size; i++) {
                if (i) printf(", ");
                printf("%c", v.data[i]);
        }
        printf("]");
}

/* ---------- BOOL ---------- */
typedef struct {
        int *data;
        size_t size;
        size_t capacity;
} vec_bool;

static inline vec_bool __pinum_vec_bool_init(int count, ...) {
        vec_bool v = {NULL, 0, 0};
        if (count > 0) {
                v.data = (int *)malloc(sizeof(int) * (size_t)count);
                if (v.data) {
                        v.size = v.capacity = (size_t)count;
                        va_list ap;
                        va_start(ap, count);
                        for (int i = 0; i < count; i++) {
                                v.data[i] = va_arg(ap, int);
                        }
                        va_end(ap);
                }
        }
        return v;
}
static inline void __pinum_vec_bool_append(vec_bool *v, int item) {
        if (v->size >= v->capacity) {
                size_t new_cap = v->capacity == 0 ? 4 : v->capacity * 2;
                int *new_data = (int *)realloc(v->data, sizeof(int) * new_cap);
                if (!new_data) {
                        return;
                }
                v->data = new_data;
                v->capacity = new_cap;
        }
        v->data[v->size++] = item;
}
static inline void __pinum_vec_bool_print(vec_bool v) {
        printf("[");
        for (size_t i = 0; i < v.size; i++) {
                if (i) printf(", ");
                printf("%d", v.data[i]);
        }
        printf("]");
}

/* ---------- SRING ---------- */
typedef struct {
        char **data;
        size_t size;
        size_t capacity;
} vec_string;

static inline vec_string __pinum_vec_string_init(int count, ...) {
        vec_string v = {NULL, 0, 0};
        if (count > 0) {
                v.data = (char **)malloc(sizeof(char *) * (size_t)count);
                if (v.data) {
                        v.size = v.capacity = (size_t)count;
                        va_list ap;
                        va_start(ap, count);
                        for (int i = 0; i < count; i++) {
                                v.data[i] = va_arg(ap, char *);
                        }
                        va_end(ap);
                }
        }
        return v;
}
static inline void __pinum_vec_string_append(vec_string *v, char *item) {
        if (v->size >= v->capacity) {
                size_t new_cap = v->capacity == 0 ? 4 : v->capacity * 2;
                char **new_data = (char **)realloc(v->data, sizeof(char *) * new_cap);
                if (!new_data) {
                        return;
                }
                v->data = new_data;
                v->capacity = new_cap;
        }
        v->data[v->size++] = item;
}
static inline void __pinum_vec_string_print(vec_string v) {
        printf("[");
        for (size_t i = 0; i < v.size; i++) {
                if (i) printf(", ");
                printf("%s", v.data[i]);
        }
        printf("]");
}

#endif // !PINUM_RTVEC_H
