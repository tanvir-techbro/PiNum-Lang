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

#include "../include/codegen_c.h"
#include <stdlib.h>

// for symbol table
static const char **g_sym_names = NULL;
static const char **g_sym_types = NULL;
static int g_sym_count = 0;
static int g_sym_capacity = 0;

static void sym_register(const char *name, const char *type) {
        // dynamic size allocation
        if (g_sym_count >= g_sym_capacity) {
                // first time assign the capacity to 16
                g_sym_capacity = g_sym_capacity ? g_sym_capacity * 2 : 16;
                g_sym_names = realloc(g_sym_names, g_sym_capacity * sizeof(char *));
                g_sym_types = realloc(g_sym_types, g_sym_capacity * sizeof(char *));
        }
        g_sym_names[g_sym_count] = name;
        g_sym_types[g_sym_count] = strdup(type); // ours to free, so local buffers are safe
        g_sym_count++;
}
static const char *sym_type_of(const char *name) {
        for (int i = 0; i < g_sym_count; i++) {
                if (strcmp(g_sym_names[i], name) == 0) {
                        return g_sym_types[i];
                }
        }
        return NULL;
}

static const char *specifier_for_type(const char *type) {
        if (strcmp(type, "char *") == 0) return "%s";
        if (strcmp(type, "long double") == 0) return "%Lf";
        if (strcmp(type, "long int") == 0) return "%ld";
        if (strcmp(type, "unsigned int") == 0) return "%u";
        if (strcmp(type, "double") == 0) return "%lf";
        if (strcmp(type, "float") == 0) return "%f";
        if (strcmp(type, "char") == 0) return "%c";
        return "%d"; // int, bool, and everything else
}

// returns the printf format specifier that matches a node's value type
static const char *codegen_specifier(ASTnode *node) {
        switch (node->type) {
        case NODE_STRING_LITERAL: return "%s";
        case NODE_FLOAT_LITERAL: return "%f";
        case NODE_CHAR_LITERAL: return "%c";
        case NODE_BOOL_LITERAL: return "%d"; // true/false printed as 1/0
        case NODE_TERNARY_EXPRESSION: return codegen_specifier(node->data.ternary_expression.then_expr);
        case NODE_IDENTIFIER: {
                const char *type = sym_type_of(node->data.identifier.name);
                return type ? specifier_for_type(type) : "%d";
        }
        default:
                return "%d";
        }
}

// maps a PiNum type name to its C equivalent
static const char *codegen_type(const char *type_name) {
        if (strcmp(type_name, "string") == 0) return "char *";
        return type_name; // int, float, double, char, bool map 1:1
}

// maps a PiNum operator token to its C equivalent
static const char *codegen_operator(tokenType op) {
        switch (op) {
        case TOKEN_PLUS: return "+";
        case TOKEN_MINUS: return "-"; // can be used as unary or binary operator
        case TOKEN_STAR: return "*";
        case TOKEN_FSLASH: return "/";
        case TOKEN_PERCENT: return "%";
        case TOKEN_EEQUAL: return "==";
        case TOKEN_NEQUAL: return "!=";
        case TOKEN_LABRACKET: return "<";
        case TOKEN_RABRACKET: return ">";
        case TOKEN_LEQUAL: return "<=";
        case TOKEN_GEQUAL: return ">=";
        case TOKEN_AND: return "&&";
        case TOKEN_OR: return "||";
        case TOKEN_EXCLAMATION: return "!"; // unary operator
        default:
                return lexer_token_type_to_string(op);
        }
}

// takes one AST node and writes that node's C code to the file
static void codegen_node(ASTnode *node, FILE *output, int level) {
        switch (node->type) {

        // ---- Literals & identifiers ----
        case NODE_INT_LITERAL:
                fprintf(output, "%d", node->data.int_literal.value);
                break;
        case NODE_FLOAT_LITERAL:
                fprintf(output, "%f", node->data.float_literal.value);
                break;
        case NODE_STRING_LITERAL:
                fprintf(output, "\"%s\"", node->data.string_literal.value);
                break;
        case NODE_BOOL_LITERAL:
                fprintf(output, "%d", node->data.bool_literal.value);
                break;
        case NODE_CHAR_LITERAL:
                fprintf(output, "'%c'", node->data.char_literal.value);
                break;
        case NODE_IDENTIFIER:
                fprintf(output, "%s", node->data.identifier.name);
                break;

        // ---- Expressions ----
        case NODE_BINARY_EXPRESSION:
                // put in brakets to keep the order
                fprintf(output, "(");
                // left (op) right
                codegen_node(node->data.binary_expression.left, output, level);             // get left value
                fprintf(output, " %s ", codegen_operator(node->data.binary_expression.op)); // print operator
                codegen_node(node->data.binary_expression.right, output, level);            // get right value
                fprintf(output, ")");
                break;
        case NODE_UNARY_EXPRESSION:
                fprintf(output, "(");
                // get operator
                fprintf(output, "%s", codegen_operator(node->data.unary_expression.op));
                codegen_node(node->data.unary_expression.left, output, level);
                fprintf(output, ")");
                break;
        case NODE_TERNARY_EXPRESSION:
                fprintf(output, "(");
                codegen_node(node->data.ternary_expression.condition, output, level);
                fprintf(output, "?");
                codegen_node(node->data.ternary_expression.then_expr, output, level);
                fprintf(output, ":");
                codegen_node(node->data.ternary_expression.else_expr, output, level);
                fprintf(output, ")");
                break;

        // ---- Declarations & assignment ----
        case NODE_VAR_DECL: {
                // build the full C type, e.g. "long int" or "char *"
                const char *base_type = codegen_type(node->data.var_decl.type_name);
                char full_type[64];
                if (node->data.var_decl.modifiers) {
                        snprintf(full_type, sizeof(full_type), "%s %s", node->data.var_decl.modifiers, base_type);
                } else {
                        snprintf(full_type, sizeof(full_type), "%s", base_type);
                }
                sym_register(node->data.var_decl.name, full_type);
                // get the modifier
                if (node->data.var_decl.modifiers) {
                        fprintf(output, "%s ", node->data.var_decl.modifiers);
                }
                // get type and name
                fprintf(output, "%s %s", base_type, node->data.var_decl.name);
                // check if they have any value assigned
                if (node->data.var_decl.value) {
                        fprintf(output, " = ");
                        codegen_node(node->data.var_decl.value, output, level);
                }
                fprintf(output, ";\n");
                break;
        }
        case NODE_ASSIGN:
                fprintf(output, "%s = ", node->data.assign.name);
                codegen_node(node->data.assign.value, output, level);
                fprintf(output, ";\n");
                break;

        // ---- Statements (control flow) ----
        case NODE_BLOCK: {
                fprintf(output, "{\n");
                for (int i = 0; i < node->data.blocks.count; i++) {
                        codegen_node(node->data.blocks.statements[i], output, level + 1);
                }
                fprintf(output, "}\n");
                break;
        }
        case NODE_IF_STAT:
                fprintf(output, "if (");
                codegen_node(node->data.if_stat.condition, output, level);
                fprintf(output, ") ");
                codegen_node(node->data.if_stat.then_block, output, level);
                if (node->data.if_stat.else_block) {
                        fprintf(output, " else ");
                        codegen_node(node->data.if_stat.else_block, output, level);
                }
                break;
        case NODE_WHILE:
                fprintf(output, "while (");
                codegen_node(node->data.while_loop.condition, output, level);
                fprintf(output, ") ");
                codegen_node(node->data.while_loop.body, output, level);
                break;
        case NODE_FOR:
                break; // TODO
        case NODE_RETURN:
                fprintf(output, "return ");
                codegen_node(node->data.returns.expression, output, level);
                fprintf(output, ";\n");
                break;
        case NODE_READ: {
                const char *type = sym_type_of(node->data.read.name);
                if (type == NULL) {
                        // can't pick a format specifier without knowing the variable's type
                        fprintf(output, "// TODO: unknown type for read(%s)\n", node->data.read.name);
                        break;
                }
                fprintf(output, "scanf(\"%s\", &%s);\n", specifier_for_type(type), node->data.read.name);
                break;
        }
        case NODE_BREAK:
                fprintf(output, "break;");
                break;
        case NODE_CONTINUE:
                fprintf(output, "continue;");
                break;

        // ---- Built-in statements ----
        case NODE_PRINT:
                fprintf(output, "printf(\"");
                // gets the specifier and writes it to the print statement
                for (int i = 0; i < node->data.print.arg_count; i++) {
                        fprintf(output, "%s", codegen_specifier(node->data.print.args[i]));
                }
                fprintf(output, "\"");
                // adds args for each specifier
                for (int i = 0; i < node->data.print.arg_count; i++) {
                        fprintf(output, ", ");
                        codegen_node(node->data.print.args[i], output, level);
                }
                fprintf(output, ");\n");
                break;

        // ---- Functions ----
        case NODE_FUNC_DEF:
                break; // TODO
        case NODE_FUNC_CALL:
                break; // TODO

        // ---- Directives & other ----
        case NODE_DIRECTIVE:
                break; // TODO
        case NODE_IMPORT:
                break; // TODO

        default:
                fprintf(output, "//TODO: %s\n", node_type_name(node->type));
        }
}

// --- MAIN ---
void codegen_c(ASTnode *program, FILE *output) {
        char *home_dir = getenv("HOME");
        fprintf(output, "#include \"%s/.pinum-lang/runtime/pinum_runtime.h\"\n", home_dir);
        fprintf(output, "int main(void) {\n");
        for (int i = 0; i < program->data.program.count; i++) {
                codegen_node(program->data.program.statements[i], output, 1);
        }
        fprintf(output, "\nreturn 0;\n}\n");

        // free the arrays and the type strings we allocated
        for (int i = 0; i < g_sym_count; i++) {
                free((void *)g_sym_types[i]);
        }
        free(g_sym_names);
        free(g_sym_types);
        g_sym_count = 0;
        g_sym_capacity = 0;
}
