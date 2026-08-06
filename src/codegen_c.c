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
#include <stdio.h>

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
        case NODE_IDENTIFIER:
                // TODO: needs a symbol table to know the variable's type
                return "%d";
        default:
                return "%d";
        }
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
                // get the modifier
                if (node->data.var_decl.modifiers) {
                        fprintf(output, "%s", node->data.var_decl.modifiers);
                }
                // get type and name
                fprintf(output, "%s %s", node->data.var_decl.type_name, node->data.var_decl.name);
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
        fprintf(output, "int main(void) {\n");
        for (int i = 0; i < program->data.program.count; i++) {
                codegen_node(program->data.program.statements[i], output, 1);
        }
        fprintf(output, "\nreturn 0;\n}\n");
}
