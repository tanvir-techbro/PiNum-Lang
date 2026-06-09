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

#ifndef AST_H
#define AST_H

#include "lexer.h"

// Possible nodetypes parser can build
typedef enum {
        NODE_PROGRAM,   // root node containing all statements
        NODE_VAR_DECL,  // declaring a veriable
        NODE_ASSIGN,    // modifying veriable, eg. x = x + 1
        NODE_FUNC_CALL, // function call
        NODE_FUNC_DEF,  // function definition
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
        NODE_WHILE,   // while loop
        NODE_FOR,     // for loop
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
                        char *type_name; // e.g., "int", "float"
                        char *modifiers; // e.g., "unsigned", "long"
                        char *name;      // veriable name
                        ASTnode *value;  // pointer to the expression node or value (can be NULL)
                        bool is_array;
                        int array_size; // size if it's an array, 0 otherwise
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
                struct {
                        char *return_type;
                        char *name;
                        ASTnode **params; // array of NODE_VAR_DECL
                        int param_count;
                        ASTnode *body; // NODE_BLOCK
                } func_def;

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
                        ASTnode *right;
                } binary_expression;
                struct {
                        tokenType op;
                        ASTnode *left;
                } unary_expression;

                // --- Control flow and blocks ---
                struct {
                        ASTnode **statements;
                        int count;
                        int capacity;
                } blocks;
                struct {
                        ASTnode *condition;
                        ASTnode *then_block;
                        ASTnode *else_block; // can be NULL
                } if_stat;
                struct {
                        ASTnode *condition;
                        ASTnode *body;
                } while_loop;
                struct {
                        ASTnode *init;
                        ASTnode *condition;
                        ASTnode *increment;
                        ASTnode *body;
                } for_loop;

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

// Creates a basic node with a specific type. Node initialization.
ASTnode *create_ast_node(nodeType type);
// specalized factory function for value assignition
ASTnode *make_int_node(int value);
ASTnode *make_float_node(double value);
ASTnode *make_string_node(char *value);
ASTnode *make_bool_node(bool value);
ASTnode *make_char_node(char value);
ASTnode *make_identifier_node(char *name);
ASTnode *make_binary_node(ASTnode *left, tokenType op, ASTnode *right);
ASTnode *make_unary_node(tokenType op, ASTnode *left);
ASTnode *make_if_stat_node(ASTnode *condition, ASTnode *then_block, ASTnode *else_block);
ASTnode *make_while_node(ASTnode *condition, ASTnode *body);
ASTnode *make_for_node(ASTnode *init, ASTnode *condition, ASTnode *increment, ASTnode *body);
ASTnode *make_var_decl_node(char *type_name, char *modifiers, char *name, ASTnode *value, bool is_array, int array_size);
ASTnode *make_assign_node(char *name, ASTnode *value);
ASTnode *make_func_call_node(char *name, ASTnode **args, int arg_count);
ASTnode *make_func_def_node(char *return_type, char *name, ASTnode **params, int param_count, ASTnode *body);
// helper functions for collection
void ast_add_statement(ASTnode *parent, ASTnode *stmt);
void ast_add_arg(ASTnode *func_call, ASTnode *arg);
void ast_add_param(ASTnode *func_def, ASTnode *param);

#endif // !AST_H
