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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * @enum nodeType
 * @brief Defines all possible types of nodes in the AST.
 */
typedef enum {
        NODE_PROGRAM,   // Root node; stores a list of top-level statements.
        NODE_VAR_DECL,  // Variable declaration (e.g., 'int x = 5').
        NODE_ASSIGN,    // Variable assignment (e.g., 'x = 10').
        NODE_FUNC_CALL, // Function invocation with arguments.
        NODE_FUNC_DEF,  // Function definition (parameters, return type, body).

        // Element Access
        NODE_MEMBER_ACCESS, // Accessing a struct/object member (e.g., 'obj.prop').
        NODE_ARRAY_ACCESS,  // Accessing an array element (e.g., 'arr[0]').

        // Literals and Identifiers
        NODE_INT_LITERAL,    // Integer constant (e.g., 42).
        NODE_FLOAT_LITERAL,  // Floating-point constant (e.g., 3.14).
        NODE_STRING_LITERAL, // String constant (e.g., "hello").
        NODE_BOOL_LITERAL,   // Boolean constant (true/false).
        NODE_CHAR_LITERAL,   // Character constant (e.g., 'a').
        NODE_IDENTIFIER,     // Variable or function name.

        // Expressions
        NODE_BINARY_EXPRESSION, // Expression with two operands (e.g., a + b).
        NODE_UNARY_EXPRESSION,  // Expression with one operand (e.g., !a, -b).

        // Control Flow and Blocks
        NODE_BLOCK,   // A group of statements enclosed in braces {}.
        NODE_IF_STAT, // If-Else conditional statement.
        NODE_WHILE,   // While loop.
        NODE_FOR,     // For loop.

        // Built-in Statements
        NODE_IMPORT, // Import/include another module.
        NODE_RETURN, // Return from a function.
        NODE_PRINT,  // Built-in print statement.
        NODE_READ    // Built-in read/input statement.
} nodeType;

// Forward declaration so the struct can reference itself
typedef struct ASTnode ASTnode;

/*
 * @struct ASTnode
 * @brief The fundamental building block of the AST.
 *
 * Each node has a type and a union containing data specific to that type.
 * This structure allows us to represent a wide variety of language constructs
 * using a single, unified type.
 */
struct ASTnode {
        nodeType type; // What kind of node this is.

        union {
                // NODE_PROGRAM: The top-level entry point.
                struct {
                        ASTnode **statements; // Dynamic array of statement nodes.
                        int count;            // Number of statements.
                        int capacity;         // Allocated size of the array.
                } program;

                // NODE_VAR_DECL: Declaring a new variable.
                struct {
                        char *type_name; // e.g., "int", "float"
                        char *modifiers; // e.g., "unsigned", "long"
                        char *name;      // Variable name
                        ASTnode *value;  // Initial value expression (can be NULL)
                        bool is_array;
                        int array_size; // Size if it's an array, 0 otherwise
                } var_decl;

                // NODE_ASSIGN: Assigning a value to an existing variable.
                struct {
                        char *name;     // Variable name
                        ASTnode *value; // New value expression
                } assign;

                // NODE_FUNC_CALL: Calling a function.
                struct {
                        char *name;     // Function name
                        ASTnode **args; // Dynamic array of argument expressions
                        int arg_count;  // Number of arguments
                } func_call;

                // NODE_FUNC_DEF: Defining a function.
                struct {
                        char *return_type;
                        char *name;
                        ASTnode **params; // Array of NODE_VAR_DECL nodes
                        int param_count;
                        ASTnode *body; // NODE_BLOCK containing the function code
                } func_def;

                // NODE_MEMBER_ACCESS: obj.member
                struct {
                        ASTnode *object;
                        char *member;
                } memeber_access;

                // NODE_ARRAY_ACCESS: arr[index]
                struct {
                        char *name;
                        ASTnode *index;
                } array_access;

                // NODE_INT_LITERAL
                struct {
                        int value;
                } int_literal;

                // NODE_FLOAT_LITERAL
                struct {
                        double value;
                } float_literal;

                // NODE_STRING_LITERAL
                struct {
                        char *value;
                } string_literal;

                // NODE_BOOL_LITERAL
                struct {
                        bool value;
                } bool_literal;

                // NODE_CHAR_LITERAL
                struct {
                        char value;
                } char_literal;

                // NODE_IDENTIFIER
                struct {
                        char *name;
                } identifier;

                // NODE_BINARY_EXPRESSION: left OP right
                struct {
                        ASTnode *left;
                        tokenType op;
                        ASTnode *right;
                } binary_expression;

                // NODE_UNARY_EXPRESSION: OP left
                struct {
                        tokenType op;
                        ASTnode *left;
                } unary_expression;

                // NODE_BLOCK: { statements... }
                struct {
                        ASTnode **statements; // Dynamic array of statement nodes
                        int count;
                        int capacity;
                } blocks;

                // NODE_IF_STAT
                struct {
                        ASTnode *condition;
                        ASTnode *then_block;
                        ASTnode *else_block; // Optional, can be NULL
                } if_stat;

                // NODE_WHILE
                struct {
                        ASTnode *condition;
                        ASTnode *body;
                } while_loop;

                // NODE_FOR
                struct {
                        ASTnode *init;      // Initialization statement
                        ASTnode *condition; // Loop condition
                        ASTnode *increment; // Increment expression
                        ASTnode *body;      // Loop body block
                } for_loop;

                // Built-in operations
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
