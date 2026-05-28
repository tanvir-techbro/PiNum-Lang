#ifndef AST_H
#define AST_H

// Possible nodetypes parser can build
typedef enum {
        NODE_PROGRAM,  // root node containing all statements
        NODE_VAR_DECL, // declaring a veriable
        NODE_ASSIGN    // modifying veriable, eg. x = x + 1
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
