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

        // Ignore white spaces, untill we hit EOF or newline
        while (ch != EOF && isspace(ch) && ch != '\n') {
                ch = fgetc(buffer);
        }

        // check for end of file (EOF)
        if (ch == EOF) {
                tokens.type = TOKEN_EOF;
                tokens.value = strdup("EOF");
                return tokens;
        }

        // ID, keywords, veriable and others handling.
        if (isalpha(ch) || ch == '_') {
                ungetc(ch, buffer);
                return lexer_tokenize_words(buffer);
        }

        // - number handling -
        if (isdigit(ch) || ch == '.') {
                ungetc(ch, buffer);
                return lexer_tokenize_numbers(buffer);
        }

        // --------------- SINGLE CHARACTER TOKEN ---------------
        switch (ch) {
        case '=':
                ch = fgetc(buffer);
                if (ch == ' ' || ch != '=') {
                        ungetc(ch, buffer);
                        tokens.type = TOKEN_EQUAL;
                        tokens.value = strdup("=");
                } else if (ch == '=') {
                        tokens.type = TOKEN_EEQUAL;
                        tokens.value = strdup("==");
                }
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
        case '<':
                ch = fgetc(buffer);
                if (ch == ' ' || ch != '=') {
                        ungetc(ch, buffer);
                        tokens.type = TOKEN_LABRACKET;
                        tokens.value = strdup("<");
                } else if (ch == '=') {
                        tokens.type = TOKEN_LEQUAL;
                        tokens.value = strdup("<=");
                }
                break;
        case '>':
                ch = fgetc(buffer);
                if (ch == ' ' || ch != '=') {
                        ungetc(ch, buffer);
                        tokens.type = TOKEN_RABRACKET;
                        tokens.value = strdup(">");
                } else if (ch == '=') {
                        tokens.type = TOKEN_GEQUAL;
                        tokens.value = strdup(">=");
                }
                break;
        case '!':
                ch = fgetc(buffer);
                if (ch == ' ' || ch != '=') {
                        ungetc(ch, buffer);
                        tokens.type = TOKEN_EXCLAMATION;
                        tokens.value = strdup("!");
                } else if (ch == '=') {
                        tokens.type = TOKEN_NEQUAL;
                        tokens.value = strdup("!=");
                }
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
                ch = fgetc(buffer);
                if (ch == ' ' || ch != '&') {
                        ungetc(ch, buffer);
                        tokens.type = TOKEN_AMPERSAND;
                        tokens.value = strdup("&");
                } else if (ch == '&') {
                        tokens.type = TOKEN_AND;
                        tokens.value = strdup("&&");
                }
                break;
        case '|':
                ch = fgetc(buffer);
                if (ch == ' ' || ch != '|') {
                        ungetc(ch, buffer);
                        tokens.type = TOKEN_PIPE;
                        tokens.value = strdup("|");
                } else if (ch == '|') {
                        tokens.type = TOKEN_OR;
                        tokens.value = strdup("||");
                }
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
        // For physically written newline, tab and null terminator;
        case '\\':
                ch = fgetc(buffer);
                if (ch == 'n') {
                        tokens.type = TOKEN_UNLINE;
                        tokens.value = strdup("\\n");
                } else if (ch == 't') {
                        tokens.type = TOKEN_TAB;
                        tokens.value = strdup("\\t");
                } else if (ch == '0') {
                        tokens.type = TOKEN_NTERMINATOR;
                        tokens.value = strdup("\\0");
                } else {
                        ungetc(ch, buffer);
                        tokens.type = TOKEN_UNKNOWN;
                        tokens.value = strdup("\\");
                }
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

// ID, keywords, veriable and others handling.
// Triggered when the current char we are reading is a letter or a non-numerical character.
token lexer_tokenize_words(FILE *buffer) {
        int ch, i = 0; // i is also used for size ditermination.
        token tokens;
        tokens.value = NULL;

        int capacity = 2; // capacity will be doubled everytime the size is close to the capacity.
        char *char_buffer = malloc(capacity);

        // - reading the characters and putting them in the char_buffer -
        ch = fgetc(buffer);

        while (ch != EOF && (isalnum(ch) || ch == '_')) {
                if (i + 1 >= capacity) {
                        capacity *= 2;
                        char_buffer = realloc(char_buffer, capacity);
                }

                char_buffer[i++] = (char)ch;
                ch = fgetc(buffer);
        }
        char_buffer[i] = '\0';

        if (ch != EOF) {
                ungetc(ch, buffer);
        }

        // - Identifying and handling ID, veriable and datatypes -
        tokens.value = char_buffer;

        if (strcmp(char_buffer, "int") == 0) {
                tokens.type = TOKEN_INT;
        } else if (strcmp(char_buffer, "float") == 0) {
                tokens.type = TOKEN_FLOAT;
        } else if (strcmp(char_buffer, "print") == 0) {
                tokens.type = TOKEN_PRINT;
        } else if (strcmp(char_buffer, "if") == 0) {
                tokens.type = TOKEN_IF;
        } else if (strcmp(char_buffer, "else") == 0) {
                tokens.type = TOKEN_ELSE;
        } else if (strcmp(char_buffer, "return") == 0) {
                tokens.type = TOKEN_RETURN;
        } else if (strcmp(char_buffer, "import") == 0) {
                tokens.type = TOKEN_IMPORT;
        } else if (strcmp(char_buffer, "read") == 0) {
                tokens.type = TOKEN_READ;
        } else if (strcmp(char_buffer, "long") == 0) {
                tokens.type = TOKEN_LONG;
        } else if (strcmp(char_buffer, "short") == 0) {
                tokens.type = TOKEN_SHORT;
        } else if (strcmp(char_buffer, "signed") == 0) {
                tokens.type = TOKEN_SIGNED;
        } else if (strcmp(char_buffer, "unsigned") == 0) {
                tokens.type = TOKEN_UNSIGNED;
        } else if (strcmp(char_buffer, "bool") == 0) {
                tokens.type = TOKEN_BOOL;
        } else if (strcmp(char_buffer, "double") == 0) {
                tokens.type = TOKEN_DOUBLE;
        } else if (strcmp(char_buffer, "char") == 0) {
                tokens.type = TOKEN_CHAR;
        } else if (strcmp(char_buffer, "string") == 0) {
                tokens.type = TOKEN_STRING;
        } else if (strcmp(char_buffer, "true") == 0) {
                tokens.type = TOKEN_TRUE;
        } else if (strcmp(char_buffer, "false") == 0) {
                tokens.type = TOKEN_FALSE;
        }
        /* LIBRERIES */
        else if (strcmp(char_buffer, "stdlib") == 0) {
                tokens.type = TOKEN_LIB_STDLIB;
        } else if (strcmp(char_buffer, "math") == 0) {
                tokens.type = TOKEN_LIB_MATH;
        }
        /* If nothing matches */
        else {
                tokens.type = TOKEN_ID;
        }

        return tokens;
}

// number tokenization, triggered when stumbled upon a digit.
token lexer_tokenize_numbers(FILE *buffer) {
        int ch, i = 0;
        token tokens;
        tokens.value = NULL;
        char char_buffer[64];

        // would be true if there is a dot in the number.
        bool is_float = false;

        // - reading the numbers and putting them in the char_buffer -
        ch = fgetc(buffer);

        while (ch != EOF && (isdigit(ch) || ch == '.')) {
                if (ch == '.') {
                        // break if we hit second dot in the sngle number.
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
        case TOKEN_TRUE:
                return "TOKEN_TRUE";
        case TOKEN_FALSE:
                return "TOKEN_FALSE";
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
        case TOKEN_LABRACKET:
                return "TOKEN_LABRACKET";
        case TOKEN_RABRACKET:
                return "TOKEN_RABRACKET";
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
        case TOKEN_AND:
                return "TOKEN_AND";
        case TOKEN_PIPE:
                return "TOKEN_PIPE";
        case TOKEN_OR:
                return "TOKEN_OR";
        case TOKEN_QUESTION:
                return "TOKEN_QUESTION";
        case TOKEN_TILDE:
                return "TOKEN_TILDE";
        case TOKEN_SQUOTE:
                return "TOKEN_SQUOTE";
        case TOKEN_DQUOTE:
                return "TOKEN_DQUOTE";
        case TOKEN_EEQUAL:
                return "TOKEN_EEQUAL";
        case TOKEN_NEQUAL:
                return "TOKEN_NEQUAL";
        case TOKEN_LEQUAL:
                return "TOKEN_LEQUAL";
        case TOKEN_GEQUAL:
                return "TOKEN_GEQUAL";
        case TOKEN_NTERMINATOR:
                return "TOKEN_NTERMINATOR";
        case TOKEN_NLINE:
                return "TOKEN_NLINE";
        case TOKEN_UNLINE:
                return "TOKEN_UNLINE";
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
