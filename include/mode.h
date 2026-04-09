#ifndef MODE_H
#define MODE_H

#include "lexer.h"
#include <stdbool.h>

// checks if ENGINE_MODE can be enabled or not
bool check_program_mode(token_list *list);

#endif // !MODE_H

// NOTE: All the function in this file will be defined in ../src/helper.c
