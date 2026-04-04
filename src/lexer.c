#include "../include/lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This function tokenizes all the words, keywords and characters in the provided .pn file.
// The tokens are then handed to the parser to be grammer checked.
token lexer_tokenizer(FILE *buffer) {
        token tokens;
        tokens.value = NULL;
        int ch = fgetc(buffer);

        // Ignore white spaces
        while (ch != EOF && isspace(ch)) {
                ch = fgetc(buffer);
        }

        // check for end of file (EOF)
        if (ch == EOF) {
                tokens.type = TOKEN_EOF;
                tokens.value = strdup("EOF");
                return tokens;
        }

        // - Veriable handling -
        if (isalpha(ch) || ch == '_') {
                // TODO: Implement word/keyword tokenization
        }

        // - number handling -
        if (isdigit(ch) || ch == '.') {
                ungetc(ch, buffer);
                return lexer_tokenize_numbers(buffer);
        }

        // --------------- SINGLE CHARACTER TOKEN ---------------
        switch (ch) {
        case '=':
                tokens.type = TOKEN_EQUAL;
                tokens.value = strdup("=");
                break;
        case '+':
                tokens.type = TOKEN_PLUS;
                tokens.value = strdup("+");
                break;
        case '-':
                tokens.type = TOKEN_MINUS;
                tokens.value = strdup("-");
                break;
        case '*':
                tokens.type = TOKEN_STAR;
                tokens.value = strdup("*");
                break;
        case '/':
                tokens.type = TOKEN_FSLASH;
                tokens.value = strdup("/");
                break;
        case ',':
                tokens.type = TOKEN_COMMA;
                tokens.value = strdup(",");
                break;
        case ';':
                tokens.type = TOKEN_SEMICOLON;
                tokens.value = strdup(";");
                break;
        case '(':
                tokens.type = TOKEN_LRPAREN;
                tokens.value = strdup("(");
                break;
        case ')':
                tokens.type = TOKEN_RRPAREN;
                tokens.value = strdup(")");
                break;
        case '{':
                tokens.type = TOKEN_LCPAREN;
                tokens.value = strdup("{");
                break;
        case '}':
                tokens.type = TOKEN_RCPAREN;
                tokens.value = strdup("}");
                break;
        case '[':
                tokens.type = TOKEN_LSPAREN;
                tokens.value = strdup("[");
                break;
        case ']':
                tokens.type = TOKEN_RSPAREN;
                tokens.value = strdup("]");
                break;
        case '!':
                tokens.type = TOKEN_EXCLAMATION;
                tokens.value = strdup("!");
                break;
        case '@':
                tokens.type = TOKEN_ATSIGN;
                tokens.value = strdup("@");
                break;
        case '#':
                tokens.type = TOKEN_HASHTAG;
                tokens.value = strdup("#");
                break;
        case '$':
                tokens.type = TOKEN_DOLLAR;
                tokens.value = strdup("$");
                break;
        case '%':
                tokens.type = TOKEN_PERCENT;
                tokens.value = strdup("%");
                break;
        case '^':
                tokens.type = TOKEN_CARET;
                tokens.value = strdup("^");
                break;
        case '&':
                tokens.type = TOKEN_AMPERSAND;
                tokens.value = strdup("&");
                break;
        case '?':
                tokens.type = TOKEN_QUESTION;
                tokens.value = strdup("?");
                break;
        case '~':
                tokens.type = TOKEN_TILDE;
                tokens.value = strdup("~");
                break;
        // SPECIAL CHARACTER TOKENS
        case '\'':
                tokens.type = TOKEN_SQUOTE;
                tokens.value = strdup("'");
                break;
        case '\"':
                tokens.type = TOKEN_DQUOTE;
                tokens.value = strdup("\"");
                break;
        case '\0':
                tokens.type = TOKEN_NTERMINATOR;
                tokens.value = strdup("\\0");
                break;
        case '\n':
                tokens.type = TOKEN_NLINE;
                tokens.value = strdup("\\n");
                break;
        case '\t':
                tokens.type = TOKEN_TAB;
                tokens.value = strdup("\\t");
                break;
        default:
                tokens.type = TOKEN_UNKNOWN;
                tokens.value = (char *)malloc(2);
                tokens.value[0] = (char)ch;
                tokens.value[1] = '\0';
                break;
        }
        // ------------------------------------------------------

        return tokens;
}

token lexer_tokenize_numbers(FILE *buffer) {
        int ch, i = 0;
        token tokens;
        tokens.value = NULL;
        char char_buffer[64];

        bool is_float = false;

        // --- reading the numbers and putting them in the char_buffer ---
        ch = fgetc(buffer);

        while (ch != EOF && (isdigit(ch) || ch == '.')) {
                if (ch == '.') {
                        if (is_float) {
                                break;
                        }
                        is_float = true;
                }

                char_buffer[i++] = (char)ch;
                ch = fgetc(buffer);
        }
        char_buffer[i] = '\0';

        // unreading the last non digit digit char read by the loop before
        if (ch != EOF) {
                ungetc(ch, buffer);
        }
        // -----------------------------------------------------------------

        // - Number handling -
        // int_value and float_value members are only used for storing numbers which is handled by this block of code below
        if (strcmp(char_buffer, ".") == 0) {
                tokens.type = TOKEN_DOT;
                tokens.value = strdup(".");
                return tokens;
        }
        if (is_float) {
                tokens.type = TOKEN_FNUM;
                tokens.float_value = atof(char_buffer);
                return tokens;
        } else {
                tokens.type = TOKEN_INUM;
                tokens.int_value = atoi(char_buffer);
                return tokens;
        }

        return tokens;
}

// NOTE: this function is temporary and is only for debugging purposes.
const char *lexer_token_type_to_string(tokenType type) {
        switch (type) {
        case TOKEN_VERIABLE:
                return "TOKEN_VERIABLE";
        case TOKEN_INUM:
                return "TOKEN_INUM";
        case TOKEN_FNUM:
                return "TOKEN_FNUM";
        case TOKEN_ID:
                return "TOKEN_ID";
        case TOKEN_IMPORT:
                return "TOKEN_IMPORT";
        case TOKEN_LIB_STDLIB:
                return "TOKEN_LIB_STDLIB";
        case TOKEN_LIB_MATH:
                return "TOKEN_LIB_MATH";
        case TOKEN_IF:
                return "TOKEN_IF";
        case TOKEN_ELSE:
                return "TOKEN_ELSE";
        case TOKEN_PRINT:
                return "TOKEN_PRINT";
        case TOKEN_READ:
                return "TOKEN_READ";
        case TOKEN_RETURN:
                return "TOKEN_RETURN";
        case TOKEN_LONG:
                return "TOKEN_LONG";
        case TOKEN_SHORT:
                return "TOKEN_SHORT";
        case TOKEN_SIGNED:
                return "TOKEN_SIGNED";
        case TOKEN_UNSIGNED:
                return "TOKEN_UNSIGNED";
        case TOKEN_BOOL:
                return "TOKEN_BOOL";
        case TOKEN_INT:
                return "TOKEN_INT";
        case TOKEN_FLOAT:
                return "TOKEN_FLOAT";
        case TOKEN_DOUBLE:
                return "TOKEN_DOUBLE";
        case TOKEN_CHAR:
                return "TOKEN_CHAR";
        case TOKEN_STRING:
                return "TOKEN_STRING";
        case TOKEN_DOT:
                return "TOKEN_DOT";
        case TOKEN_EQUAL:
                return "TOKEN_EQUAL";
        case TOKEN_PLUS:
                return "TOKEN_PLUS";
        case TOKEN_MINUS:
                return "TOKEN_MINUS";
        case TOKEN_STAR:
                return "TOKEN_STAR";
        case TOKEN_FSLASH:
                return "TOKEN_FSLASH";
        case TOKEN_COMMA:
                return "TOKEN_COMMA";
        case TOKEN_SEMICOLON:
                return "TOKEN_SEMICOLON";
        case TOKEN_LRPAREN:
                return "TOKEN_LRPAREN";
        case TOKEN_RRPAREN:
                return "TOKEN_RRPAREN";
        case TOKEN_LCPAREN:
                return "TOKEN_LCPAREN";
        case TOKEN_RCPAREN:
                return "TOKEN_RCPAREN";
        case TOKEN_LSPAREN:
                return "TOKEN_LSPAREN";
        case TOKEN_RSPAREN:
                return "TOKEN_RSPAREN";
        case TOKEN_EXCLAMATION:
                return "TOKEN_EXCLAMATION";
        case TOKEN_ATSIGN:
                return "TOKEN_ATSIGN";
        case TOKEN_HASHTAG:
                return "TOKEN_HASHTAG";
        case TOKEN_DOLLAR:
                return "TOKEN_DOLLAR";
        case TOKEN_PERCENT:
                return "TOKEN_PERCENT";
        case TOKEN_CARET:
                return "TOKEN_CARET";
        case TOKEN_AMPERSAND:
                return "TOKEN_AMPERSAND";
        case TOKEN_QUESTION:
                return "TOKEN_QUESTION";
        case TOKEN_TILDE:
                return "TOKEN_TILDE";
        case TOKEN_SQUOTE:
                return "TOKEN_SQUOTE";
        case TOKEN_DQUOTE:
                return "TOKEN_DQUOTE";
        case TOKEN_NTERMINATOR:
                return "TOKEN_NTERMINATOR";
        case TOKEN_NLINE:
                return "TOKEN_NLINE";
        case TOKEN_TAB:
                return "TOKEN_TAB";
        case TOKEN_EOF:
                return "TOKEN_EOF";
        case TOKEN_UNKNOWN:
                return "TOKEN_UNKNOWN";
        default:
                return "UNKNOWN_TYPE";
        }
}

// NOTE: this function is temporary and is only for debugging purposes.
void lexer_print_token(token t) {
        printf("%-20s: ", lexer_token_type_to_string(t.type));
        if (t.type == TOKEN_INUM) {
                printf("%d\n", t.int_value);
        } else if (t.type == TOKEN_FNUM) {
                printf("%f\n", t.float_value);
        } else if (t.value) {
                printf("%s\n", t.value);
        } else {
                printf("\n");
        }
}
