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
        g_sym_types[g_sym_count] = type;
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
        if (strcmp(type, "string") == 0) return "%s";
        if (strcmp(type, "float") == 0 || strcmp(type, "double") == 0) return "%f";
        if (strcmp(type, "char") == 0) return "%c";
        return "%d"; // int, bool, and everything else
}

// returns the printf format specifier that matches a node's value type
static const char *codegen_specifier(ASTnode *node) {
        switch (node->type) {
        case NODE_STRING_LITERAL:
                return "%s";
        case NODE_FLOAT_LITERAL:
                return "%f";
        case NODE_CHAR_LITERAL:
                return "%c";
        case NODE_BOOL_LITERAL:
                return "%d"; // true/false printed as 1/0
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

// takes one AST node and writes that node's C code to the file
static void codegen_node(ASTnode *node, FILE *output, int level) {
        switch (node->type) {
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
        case NODE_VAR_DECL:
                sym_register(node->data.var_decl.name, node->data.var_decl.type_name);
                // get the modifier
                if (node->data.var_decl.modifiers) {
                        fprintf(output, "%s ", node->data.var_decl.modifiers);
                }
                // get type and name
                fprintf(output, "%s %s", codegen_type(node->data.var_decl.type_name), node->data.var_decl.name);
                // check if they have any value assigned
                if (node->data.var_decl.value) {
                        fprintf(output, " = ");
                        codegen_node(node->data.var_decl.value, output, level);
                }
                fprintf(output, ";\n");
                break;
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
        default:
                fprintf(output, "//TODO: %s\n", node_type_name(node->type));
        }
}

// --- MAIN ---
void codegen(ASTnode *program, FILE *output) {
        fprintf(output, "#include <stdio.h>\n");
        fprintf(output, "#include <string.h>\n");
        fprintf(output, "#include <stdbool.h>\n");
        fprintf(output, "#include <stdlib.h>\n");
        fprintf(output, "int main(void) {\n");
        for (int i = 0; i < program->data.program.count; i++) {
                codegen_node(program->data.program.statements[i], output, 1);
        }
        fprintf(output, "\nreturn 0;\n}\n");

        // free the arrays we allocated
        free(g_sym_names);
        free(g_sym_types);
        g_sym_count = 0;
}
