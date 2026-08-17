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

/* Semaintic Analysis + Symbol table */

#include "../include/sema.h"
#include "../include/error.h"
#include <stdio.h>
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
static void sem_declare(SemAnalyzer *a, const char *name, const char *type, int line, int col) {
        // previous scope / frame
        HashMap *frame = a->frames[a->frame_count - 1];
        char *k = strdup(name);
        char *v = strdup(type);
        // returns false if key exists
        if (!hashmap_insert(frame, k, v)) {
                free(k);
                free(v);
                pinum_error_at(STAGE_SEMANTIC, ERR_REDECLARED_VAR, line, col, name);
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
// full C type for a symbol, e.g. "long int", "char *", "vec", "vec_int"
static char *sem_fulltype(const char *type_name, const char *modifiers, const char *element_type) {
        char *base;
        if (strcmp(type_name, "string") == 0) base = strdup("char *");
        else if (strcmp(type_name, "vec") == 0) {
                // monomorphize vec<T> → vec_T
                char buf[32];
                snprintf(buf, sizeof(buf), "vec_%s", element_type ? element_type : "int");
                base = strdup(buf);
        } else base = strdup(type_name);
        if (modifiers == NULL) {
                return base;
        }
        char *out = malloc(strlen(modifiers) + strlen(base) + 2);
        sprintf(out, "%s %s", modifiers, base);
        free(base);
        return out;
}

// - Walker -
static void sem_analyze_node(SemAnalyzer *a, ASTnode *node) {
        switch (node->type) {

        // ---- Program & blocks ----
        case NODE_PROGRAM:
                sem_push_scope(a);
                for (int i = 0; i < node->data.program.count; i++) {
                        sem_analyze_node(a, node->data.program.statements[i]);
                }
                sem_pop_scope(a);
                break;
        case NODE_BLOCK:
                sem_push_scope(a);
                for (int i = 0; i < node->data.blocks.count; i++) {
                        sem_analyze_node(a, node->data.blocks.statements[i]);
                }
                sem_pop_scope(a);
                break;

        // ---- Literals (nothing to check) ----
        case NODE_INT_LITERAL: break;
        case NODE_FLOAT_LITERAL: break;
        case NODE_STRING_LITERAL: break;
        case NODE_BOOL_LITERAL: break;
        case NODE_CHAR_LITERAL: break;
        case NODE_LIST_LITERAL: {
                for (int i = 0; i < node->data.list_literal.count; i++) {
                        sem_analyze_node(a, node->data.list_literal.elements[i]);
                }
                break;
        }

        // ---- Identifiers & element access ----
        case NODE_IDENTIFIER: {
                const char *type = sem_resolve(a, node->data.identifier.name);
                if (!type) {
                        pinum_error_at(STAGE_SEMANTIC, ERR_UNDECLARED_VAR, node->line, node->col, node->data.identifier.name);
                }
                node->resolved_type = strdup(type);
                break;
        }
        case NODE_ARRAY_ACCESS: {
                const char *type = sem_resolve(a, node->data.array_access.name);
                if (!type) {
                        pinum_error_at(STAGE_SEMANTIC, ERR_UNDECLARED_VAR, node->line, node->col, node->data.array_access.name);
                }
                node->resolved_type = strdup(type);
                sem_analyze_node(a, node->data.array_access.index);
                break;
        }
        case NODE_MEMBER_ACCESS:
                sem_analyze_node(a, node->data.memeber_access.object);
                break;

        // ---- Expressions (binary / unary / ternary) ----
        case NODE_BINARY_EXPRESSION:
                sem_analyze_node(a, node->data.binary_expression.left);
                sem_analyze_node(a, node->data.binary_expression.right);
                break;
        case NODE_UNARY_EXPRESSION:
                sem_analyze_node(a, node->data.unary_expression.left);
                break;
        case NODE_TERNARY_EXPRESSION:
                sem_analyze_node(a, node->data.ternary_expression.condition);
                sem_analyze_node(a, node->data.ternary_expression.then_expr);
                sem_analyze_node(a, node->data.ternary_expression.else_expr);
                break;

        // ---- Declarations & assignment ----
        case NODE_VAR_DECL: {
                char *type = sem_fulltype(node->data.var_decl.type_name, node->data.var_decl.modifiers, node->data.var_decl.element_type);
                sem_declare(a, node->data.var_decl.name, type, node->line, node->col);
                free(type); // sem_declare strdup'd it so we can free this copy
                if (node->data.var_decl.value) {
                        sem_analyze_node(a, node->data.var_decl.value);
                        // give a [ ... ] literal in a vec<T> decl its concrete type
                        if (node->data.var_decl.value->type == NODE_LIST_LITERAL && node->data.var_decl.element_type) {
                                char buf[32];
                                snprintf(buf, sizeof(buf), "vec_%s", node->data.var_decl.element_type);
                                node->data.var_decl.value->resolved_type = strdup(buf);
                        }
                }
                break;
        }
        case NODE_ASSIGN: {
                const char *type = sem_resolve(a, node->data.assign.name);
                if (!type) {
                        pinum_error_at(STAGE_SEMANTIC, ERR_UNDECLARED_VAR, node->line, node->col, node->data.assign.name);
                }
                sem_analyze_node(a, node->data.assign.value);
                break;
        }

        // ---- Statements (control flow) ----
        case NODE_IF_STAT:
                sem_analyze_node(a, node->data.if_stat.condition);
                sem_analyze_node(a, node->data.if_stat.then_block);
                if (node->data.if_stat.else_block) {
                        sem_analyze_node(a, node->data.if_stat.else_block);
                }
                break;
        case NODE_WHILE:
                sem_analyze_node(a, node->data.while_loop.condition);
                sem_analyze_node(a, node->data.while_loop.body);
                break;
        case NODE_FOR:
                sem_push_scope(a);
                if (node->data.for_loop.init) {
                        sem_analyze_node(a, node->data.for_loop.init);
                }
                if (node->data.for_loop.condition) {
                        sem_analyze_node(a, node->data.for_loop.condition);
                }
                if (node->data.for_loop.increment) {
                        sem_analyze_node(a, node->data.for_loop.increment);
                }
                sem_analyze_node(a, node->data.for_loop.body);
                sem_pop_scope(a);
                break;
        case NODE_RETURN:
                if (node->data.returns.expression) {
                        sem_analyze_node(a, node->data.returns.expression);
                }
                break;
        case NODE_READ: {
                const char *type = sem_resolve(a, node->data.read.name);
                if (!type) {
                        pinum_error_at(STAGE_SEMANTIC, ERR_UNDECLARED_VAR, node->line, node->col, node->data.read.name);
                }
                node->resolved_type = strdup(type);
                break;
        }
        case NODE_BREAK: break;
        case NODE_CONTINUE: break;

        // ---- Built-in statements ----
        case NODE_PRINT:
                for (int i = 0; i < node->data.print.arg_count; i++) {
                        sem_analyze_node(a, node->data.print.args[i]);
                }
                break;

        // ---- Functions ----
        case NODE_FUNC_DEF:
                sem_push_scope(a);
                for (int i = 0; i < node->data.func_def.param_count; i++) {
                        sem_analyze_node(a, node->data.func_def.params[i]);
                }
                if (node->data.func_def.body) {
                        sem_analyze_node(a, node->data.func_def.body);
                }
                sem_pop_scope(a);
                break;
        case NODE_FUNC_CALL:
                for (int i = 0; i < node->data.func_call.arg_count; i++) {
                        sem_analyze_node(a, node->data.func_call.args[i]);
                }
                break;

        // ---- Directives & other ----
        case NODE_IMPORT: break;
        case NODE_DIRECTIVE: break;

        default:
                break;
        }
}

// --- MAIN ---
void semantic_analyze(ASTnode *program) {
        SemAnalyzer a = {0};
        sem_analyze_node(&a, program);
        free(a.frames);
}
