#include "../include/error.h"

void pinum_error(ErrorCode code, const char *detail) {
        fprintf(stderr, "\033[1;31mPiNum Error:\033[0m ");

        switch (code) {
        case ERR_UNEXPECTED_TOKEN:
                fprintf(stderr, "Unexpected token '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_EXPECTED_TYPE:
                fprintf(stderr, "Expected data type (int, float, etc.) but found '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_EXPECTED_ID:
                fprintf(stderr, "Expected identifier but found '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_MISSING_SEMICOLON:
                fprintf(stderr, "Missing ';' after statement. Found '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_MISSING_PAREN:
                fprintf(stderr, "Missing closing parenthesis. Found '%s'\n", detail ? detail : "unknown");
                break;
        case ERR_INVALID_MODIFIER:
                fprintf(stderr, "Invalid modifier '%s'\n", detail ? detail : "unknown");
                break;
        default:
                fprintf(stderr, "An unknown error occurred.\n");
                break;
        }

        exit(EXIT_FAILURE);
}
