#include "ast.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Node Initialization ---
ASTnode *create_ast_node(nodeType type) {
        ASTnode *node = (ASTnode *)malloc(sizeof(ASTnode));
        if (node == NULL) {
                perror("Failed to allocate ast node.\n");
                exit(EXIT_FAILURE);
        }
        memset(node, 0, sizeof(ASTnode));
        node->type = type;
        return node;
}

// --- value assignition ---
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
ASTnode *make_var_decl_node(char *name, ASTnode *value) {
        ASTnode *node = create_ast_node(NODE_VAR_DECL);
        node->data.var_decl.name = strdup(name);
        node->data.var_decl.value = value;
        return node;
}
