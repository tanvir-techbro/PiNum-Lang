#ifndef AST_H
#define AST_H

#include "../include/lexer.h"

// Possible nodetypes parser can build
typedef enum {
        NODE_PROGRAM,   // root node containing all statements
        NODE_VAR_DECL,  // declaring a veriable
        NODE_ASSIGN,    // modifying veriable, eg. x = x + 1
        NODE_FUNC_CALL, // function call
        // accessing elements
        NODE_MEMBER_ACCESS, // member access
        NODE_ARRAY_ACCESS,  // array element access
        // literals and identifier nodes
        NODE_INT_LITERAL,    // intager constants, eg. 13
        NODE_FLOAT_LITERAL,  // decimal constants, eg. 6.7
        NODE_STRING_LITERAL, // quoted strings, eg. "Hello"
        NODE_BOOL_LITERAL,   // true/false or 1/0
        NODE_CHAR_LITERAL,   // single characters, eg. 'a'
        NODE_IDENTIFIER,     // to represent variable names when they show up in expressions, eg. x + 1 (where x is identifier)
        // Expressions
        NODE_BINARY_EXPRESSION, // expressions with 2 sides, eg. + - = etc.
        NODE_UNARY_EXPRESSION,  // expressions with 1 sides, eg. !isTrue (here ! is the expression)
        // control flow and blocks
        NODE_BLOCK,   // to group multiple statements inside a curly brace
        NODE_IF_STAT, // to sotre the condition, then_block and else_block
        // Built in statements/functions
        NODE_IMPORT,
        NODE_RETURN,
        NODE_PRINT,
        NODE_READ
} nodeType;

// forword declaration so the struct can reference itself
typedef struct ASTnode ASTnode;

struct ASTnode {
        nodeType type;

        union {
                // root block or group of statements
                struct {
                        ASTnode **statements;
                        int count;
                        int capacity;
                } program;
                // veriable declaration
                struct {
                        char *name;     // veriable name
                        ASTnode *value; // pointer to the expression node or value
                } var_decl;
                // value assign
                struct {
                        char *name;     // veriable name
                        ASTnode *value; // veriable value
                } assign;
                struct {
                        char *name;
                        ASTnode **args;
                        int arg_count;
                } func_call;

                // --- accessing elements ---
                struct {
                        ASTnode *object;
                        char *member;
                } memeber_access;
                struct {
                        char *name;
                        ASTnode *index;
                } array_access;

                // --- Literals and Identifier ---
                // NODE_INT_LITERAL: e.g., 42
                struct {
                        int value;
                } int_literal;
                // NODE_FLOAT_LITERAL: e.g., 3.14
                struct {
                        double value;
                } float_literal;
                // NODE_STRING_LITERAL: e.g., "Hello"
                struct {
                        char *value;
                } string_literal;
                // NODE_BOOL_LITERAL: e.g., true/false
                struct {
                        bool value;
                } bool_literal;
                // NODE_CHAR_LITERAL: e.g., 'a'
                struct {
                        char value;
                } char_literal;
                // NODE_IDENTIFIER: e.g., myVariable
                struct {
                        char *name;
                } identifier;

                // --- expressions ---
                struct {
                        ASTnode *left;
                        tokenType op;
                        ASTnode *rght;
                } binary_expression;
                struct {
                        tokenType op;
                        ASTnode *left;
                } unary_expression;

                // --- Control flow and blocks ---
                struct {
                        ASTnode **statements;
                        int count;
                } blocks;
                struct {
                        ASTnode *condition;
                        ASTnode *then_block;
                        ASTnode *else_block; // can be NULL
                } if_stat;

                // --- built in statements/functions
                struct {
                        char *lib_name;
                } import;
                struct {
                        ASTnode *expression;
                } returns;
                struct {
                        ASTnode *expression;
                } print;
                struct {
                        char *name;
                } read;
        } data;
};

#endif // !AST_H
