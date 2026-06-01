#include "../include/parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// NOTE: This parser uses recursive decent parsing method.

// ---------------------------- //
// --- Core functionalities --- //
// ---------------------------- //

// --- initialization function (main) ---
ASTnode *parse(token_list *tokens) {
        Parser parser = {tokens, 0};
        return parse_program(&parser);
}

// --- Internal parsing functions (using recursive decent) ---
ASTnode *parse_program(Parser *parser) {
        ASTnode *program = create_ast_node(NODE_PROGRAM);
        while (!check(parser, TOKEN_EOF)) {
                ast_add_statement(program, parse_statement(parser));
        }
        return program;
}
