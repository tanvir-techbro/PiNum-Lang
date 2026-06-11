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

#include "../include/ast.h"

// --- Node Initialization ---
ASTnode *create_ast_node(nodeType type) {
        ASTnode *node = (ASTnode *)malloc(sizeof(ASTnode));
        if (node == NULL) {
                perror("Failed to allocate ast node.\n"); // NOTE: for debugging, no node should fail to be allocated.
                exit(EXIT_FAILURE);
        }
        memset(node, 0, sizeof(ASTnode));
        node->type = type;
        return node;
}

// --- Specialized Factory Functions ---
ASTnode *make_int_node(int value) {
        ASTnode *node = create_ast_node(NODE_INT_LITERAL);
        node->data.int_literal.value = value;
        return node;
}
ASTnode *make_float_node(double value) {
        ASTnode *node = create_ast_node(NODE_FLOAT_LITERAL);
        node->data.float_literal.value = value;
        return node;
}
ASTnode *make_string_node(char *value) {
        ASTnode *node = create_ast_node(NODE_STRING_LITERAL);
        node->data.string_literal.value = strdup(value);
        return node;
}
ASTnode *make_bool_node(bool value) {
        ASTnode *node = create_ast_node(NODE_BOOL_LITERAL);
        node->data.bool_literal.value = value;
        return node;
}
ASTnode *make_char_node(char value) {
        ASTnode *node = create_ast_node(NODE_CHAR_LITERAL);
        node->data.char_literal.value = value;
        return node;
}
ASTnode *make_identifier_node(char *name) {
        ASTnode *node = create_ast_node(NODE_IDENTIFIER);
        node->data.identifier.name = strdup(name);
        return node;
}
ASTnode *make_binary_node(ASTnode *left, tokenType op, ASTnode *right) {
        ASTnode *node = create_ast_node(NODE_BINARY_EXPRESSION);
        node->data.binary_expression.left = left;
        node->data.binary_expression.op = op;
        node->data.binary_expression.right = right;
        return node;
}
ASTnode *make_unary_node(tokenType op, ASTnode *left) {
        ASTnode *node = create_ast_node(NODE_UNARY_EXPRESSION);
        node->data.unary_expression.op = op;
        node->data.unary_expression.left = left;
        return node;
}
ASTnode *make_if_stat_node(ASTnode *condition, ASTnode *then_block, ASTnode *else_block) {
        ASTnode *node = create_ast_node(NODE_IF_STAT);
        node->data.if_stat.condition = condition;
        node->data.if_stat.then_block = then_block;
        node->data.if_stat.else_block = else_block;
        return node;
}
ASTnode *make_while_node(ASTnode *condition, ASTnode *body) {
        ASTnode *node = create_ast_node(NODE_WHILE);
        node->data.while_loop.condition = condition;
        node->data.while_loop.body = body;
        return node;
}
ASTnode *make_for_node(ASTnode *init, ASTnode *condition, ASTnode *increment, ASTnode *body) {
        ASTnode *node = create_ast_node(NODE_FOR);
        node->data.for_loop.init = init;
        node->data.for_loop.condition = condition;
        node->data.for_loop.increment = increment;
        node->data.for_loop.body = body;
        return node;
}
ASTnode *make_var_decl_node(char *type_name, char *modifiers, char *name, ASTnode *value, bool is_array, int array_size) {
        ASTnode *node = create_ast_node(NODE_VAR_DECL);
        node->data.var_decl.type_name = type_name ? strdup(type_name) : NULL;
        node->data.var_decl.modifiers = modifiers ? strdup(modifiers) : NULL;
        node->data.var_decl.name = strdup(name);
        node->data.var_decl.value = value;
        node->data.var_decl.is_array = is_array;
        node->data.var_decl.array_size = array_size;
        return node;
}
ASTnode *make_assign_node(char *name, ASTnode *value) {
        ASTnode *node = create_ast_node(NODE_ASSIGN);
        node->data.assign.name = strdup(name);
        node->data.assign.value = value;
        return node;
}
ASTnode *make_func_call_node(char *name, ASTnode **args, int arg_count) {
        ASTnode *node = create_ast_node(NODE_FUNC_CALL);
        node->data.func_call.name = strdup(name);
        node->data.func_call.args = args;
        node->data.func_call.arg_count = arg_count;
        return node;
}
ASTnode *make_func_def_node(char *return_type, char *name, ASTnode **params, int param_count, ASTnode *body) {
        ASTnode *node = create_ast_node(NODE_FUNC_DEF);
        node->data.func_def.return_type = return_type ? strdup(return_type) : NULL;
        node->data.func_def.name = strdup(name);
        node->data.func_def.params = params;
        node->data.func_def.param_count = param_count;
        node->data.func_def.body = body;
        return node;
}

// --- Helper Functions for Collections ---
// This fucntion parses statements into node program or node block
void ast_add_statement(ASTnode *parent, ASTnode *stmt) {
        if (parent->type == NODE_PROGRAM) {
                if (parent->data.program.count >= parent->data.program.capacity) {
                        parent->data.program.capacity = parent->data.program.capacity == 0 ? 8 : parent->data.program.capacity * 2;
                        parent->data.program.statements = (ASTnode **)realloc(parent->data.program.statements, sizeof(ASTnode *) * parent->data.program.capacity);
                }
                parent->data.program.statements[parent->data.program.count++] = stmt;
        } else if (parent->type == NODE_BLOCK) {
                if (parent->data.blocks.count >= parent->data.blocks.capacity) {
                        parent->data.blocks.capacity = parent->data.blocks.capacity == 0 ? 8 : parent->data.blocks.capacity * 2;
                        parent->data.blocks.statements = (ASTnode **)realloc(parent->data.blocks.statements, sizeof(ASTnode *) * parent->data.blocks.capacity);
                }
                parent->data.blocks.statements[parent->data.blocks.count++] = stmt;
        }
}
void ast_add_arg(ASTnode *func_call, ASTnode *arg) {
        // verifying correct node type
        if (func_call->type != NODE_FUNC_CALL) {
                return;
        }
        func_call->data.func_call.args = (ASTnode **)realloc(func_call->data.func_call.args, sizeof(ASTnode *) * (func_call->data.func_call.arg_count + 1));
        func_call->data.func_call.args[func_call->data.func_call.arg_count++] = arg;
}
void ast_add_param(ASTnode *func_def, ASTnode *param) {
        // verifying correct node type
        if (func_def->type != NODE_FUNC_DEF) {
                return;
        }
        func_def->data.func_def.params = (ASTnode **)realloc(func_def->data.func_def.params, sizeof(ASTnode *) * (func_def->data.func_def.param_count + 1));
        func_def->data.func_def.params[func_def->data.func_def.param_count++] = param;
}

// --- Memory Management ---
// NOTE: It is a recursive function
void free_ast_node(ASTnode *node) {
        // If the node is already empty move on
        if (node == NULL) {
                return;
        }

        // Rules for freeing different types of nodes
        // Basically freeing specific elements in each different node types
        switch (node->type) {
        case NODE_PROGRAM:
                for (int i = 0; i < node->data.program.count; i++) {
                        free_ast_node(node->data.program.statements[i]);
                }
                free(node->data.program.statements);
                break;
        case NODE_VAR_DECL:
                free(node->data.var_decl.type_name);
                free(node->data.var_decl.modifiers);
                free(node->data.var_decl.name);
                free_ast_node(node->data.var_decl.value);
                break;
        case NODE_ASSIGN:
                free(node->data.assign.name);
                free_ast_node(node->data.assign.value);
                break;
        case NODE_FUNC_CALL:
                free(node->data.func_call.name);
                for (int i = 0; i < node->data.func_call.arg_count; i++) {
                        free_ast_node(node->data.func_call.args[i]);
                }
                free(node->data.func_call.args);
                break;
        case NODE_FUNC_DEF:
                free(node->data.func_def.return_type);
                free(node->data.func_def.name);
                for (int i = 0; i < node->data.func_def.param_count; i++) {
                        free_ast_node(node->data.func_def.params[i]);
                }
                free(node->data.func_def.params);
                free_ast_node(node->data.func_def.body);
                break;
        case NODE_MEMBER_ACCESS:
                free_ast_node(node->data.memeber_access.object);
                free(node->data.memeber_access.member);
                break;
        case NODE_ARRAY_ACCESS:
                free(node->data.array_access.name);
                free_ast_node(node->data.array_access.index);
                break;
        case NODE_STRING_LITERAL:
                free(node->data.string_literal.value);
                break;
        case NODE_IDENTIFIER:
                free(node->data.identifier.name);
                break;
        case NODE_BINARY_EXPRESSION:
                free_ast_node(node->data.binary_expression.left);
                free_ast_node(node->data.binary_expression.right);
                break;
        case NODE_UNARY_EXPRESSION:
                free_ast_node(node->data.unary_expression.left);
                break;
        case NODE_BLOCK:
                for (int i = 0; i < node->data.blocks.count; i++) {
                        free_ast_node(node->data.blocks.statements[i]);
                }
                free(node->data.blocks.statements);
                break;
        case NODE_IF_STAT:
                free_ast_node(node->data.if_stat.condition);
                free_ast_node(node->data.if_stat.then_block);
                free_ast_node(node->data.if_stat.else_block);
                break;
        case NODE_WHILE:
                free_ast_node(node->data.while_loop.condition);
                free_ast_node(node->data.while_loop.body);
                break;
        case NODE_FOR:
                free_ast_node(node->data.for_loop.init);
                free_ast_node(node->data.for_loop.condition);
                free_ast_node(node->data.for_loop.increment);
                free_ast_node(node->data.for_loop.body);
                break;
        case NODE_IMPORT:
                free(node->data.import.lib_name);
                break;
        case NODE_RETURN:
                free_ast_node(node->data.returns.expression);
                break;
        case NODE_PRINT:
                free_ast_node(node->data.print.expression);
                break;
        case NODE_READ:
                free(node->data.read.name);
                break;
        default:
                break;
        }

        free(node);
}

// --- Debugging and Visualization ---
// NOTE: this function is temporary and for debugging purposes
void print_ast(ASTnode *node, int level) {
        if (node == NULL)
                return;

        for (int i = 0; i < level; i++)
                printf("  ");

        switch (node->type) {
        case NODE_PROGRAM:
                printf("PROGRAM\n");
                for (int i = 0; i < node->data.program.count; i++) {
                        print_ast(node->data.program.statements[i], level + 1);
                }
                break;
        case NODE_VAR_DECL:
                printf("VAR_DECL: %s %s %s%s\n",
                       node->data.var_decl.modifiers ? node->data.var_decl.modifiers : "",
                       node->data.var_decl.type_name ? node->data.var_decl.type_name : "",
                       node->data.var_decl.name,
                       node->data.var_decl.is_array ? "[]" : "");
                if (node->data.var_decl.value) {
                        print_ast(node->data.var_decl.value, level + 1);
                }
                break;
        case NODE_ASSIGN:
                printf("ASSIGN: %s\n", node->data.assign.name);
                print_ast(node->data.assign.value, level + 1);
                break;
        case NODE_INT_LITERAL:
                printf("INT: %d\n", node->data.int_literal.value);
                break;
        case NODE_FLOAT_LITERAL:
                printf("FLOAT: %f\n", node->data.float_literal.value);
                break;
        case NODE_IDENTIFIER:
                printf("IDENT: %s\n", node->data.identifier.name);
                break;
        case NODE_BINARY_EXPRESSION:
                printf("BINARY_OP: %d\n", node->data.binary_expression.op);
                print_ast(node->data.binary_expression.left, level + 1);
                print_ast(node->data.binary_expression.right, level + 1);
                break;
        case NODE_IF_STAT:
                printf("IF\n");
                print_ast(node->data.if_stat.condition, level + 1);
                printf("THEN\n");
                print_ast(node->data.if_stat.then_block, level + 1);
                if (node->data.if_stat.else_block) {
                        printf("ELSE\n");
                        print_ast(node->data.if_stat.else_block, level + 1);
                }
                break;
        case NODE_WHILE:
                printf("WHILE\n");
                print_ast(node->data.while_loop.condition, level + 1);
                printf("BODY\n");
                print_ast(node->data.while_loop.body, level + 1);
                break;
        case NODE_FOR:
                printf("FOR\n");
                printf("INIT:\n");
                print_ast(node->data.for_loop.init, level + 1);
                printf("COND:\n");
                print_ast(node->data.for_loop.condition, level + 1);
                printf("INC:\n");
                print_ast(node->data.for_loop.increment, level + 1);
                printf("BODY:\n");
                print_ast(node->data.for_loop.body, level + 1);
                break;
        default:
                printf("NODE_TYPE: %d\n", node->type);
                break;
        }
}
