#include "../include/lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
