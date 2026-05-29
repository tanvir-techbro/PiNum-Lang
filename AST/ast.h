#ifndef AST_H
#define AST_H

#include "../include/lexer.h"

// Possible nodetypes parser can build
typedef enum {
        NODE_PROGRAM,  // root node containing all statements
        NODE_VAR_DECL, // declaring a veriable
        NODE_ASSIGN,   // modifying veriable, eg. x = x + 1
        // literals and identifier nodes
        NODE_INT_LITERAL,    // intager constants, eg. 13
        NODE_FLOAT_LITERAL,  // decimal constants, eg. 6.7
        NODE_STRING_LITERAL, // quoted strings, eg. "Hello"
        NODE_BOOL_LITERAL,   // true/false or 1/0
        NODE_CHAR_LITERAL,   // single characters, eg. 'a'
        NODE_IDENTIFIER      // to represent variable names when they show up in expressions, eg. x + 1 (where x is identifier)
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
        } data;
};

#endif // !AST_H
