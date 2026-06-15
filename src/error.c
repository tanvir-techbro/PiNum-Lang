#include "../include/error.h"

void pinum_error(ErrorCode code, const char *detail) {
        fprintf(stderr, "\033[1;40mpinum:\033[0m 033[1;31merror:\033[0m ");

        switch (code) {
        case ERR_UNEXPECTED_TOKEN:
                fprintf(stderr, "unexpected token '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_EXPECTED_TYPE:
                fprintf(stderr, "expected data type (int, float, etc.) but found '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_EXPECTED_ID:
                fprintf(stderr, "expected identifier but found '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_MISSING_SEMICOLON:
                fprintf(stderr, "missing ';' after statement. Found '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_MISSING_PAREN:
                fprintf(stderr, "missing closing parenthesis. Found '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_INVALID_MODIFIER:
                fprintf(stderr, "invalid modifier '%s'\n", detail ? detail : "unknown");
                break;
        default:
                fprintf(stderr, "an unknown error occurred.\n");
                break;
        }

        exit(EXIT_FAILURE);
}
