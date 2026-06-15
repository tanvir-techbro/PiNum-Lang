#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
        // Parser errors
        ERR_UNEXPECTED_TOKEN,
        ERR_EXPECTED_TYPE,
        ERR_EXPECTED_ID,
        ERR_MISSING_SEMICOLON,
        ERR_MISSING_PAREN,
        ERR_INVALID_MODIFIER
} ErrorCode;

/*
 * @brief Reports an error and exits the program.
 *
 * @param code The error code representing the type of error.
 * @param detail Extra information (like the token value) to display.
 */
void pinum_error(ErrorCode code, const char *detail);

#endif
