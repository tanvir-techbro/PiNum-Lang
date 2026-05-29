#ifndef AST_H
#define AST_H

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
        } data;
};

#endif // !AST_H
