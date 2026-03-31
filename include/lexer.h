#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// All the tokentypes
typedef enum {
        TOKEN_INT,         // int data type
        TOKEN_FLOAT,       // float data type
        TOKEN_DOUBLE,      // double data type
        TOKEN_ID,          // 'x' 'y' 'result' 'print'
        TOKEN_NUM,         // numbers. 10 50 42 etc.
        TOKEN_EQUAL,       // "=" equal
        TOKEN_PLUS,        // "+" plus
        TOKEN_MINUS,       // "-" mius
        TOKEN_STAR,        // "*" star, can be used for multiplications
        TOKEN_FSLASH,      // "/" forward slash, can be use for division
        TOKEN_COMMA,       // "," comma
        TOKEN_SEMICOLON,   // ";" semicolon
        TOKEN_LRPAREN,     // "(" left round parenthesis
        TOKEN_RRPAREN,     // ")" right round parenthesis
        TOKEN_LCPAREN,     // "{" left curly parenthesis
        TOKEN_RCPAREN,     // "}" right curly parenthesis
        TOKEN_LSPAREN,     // "[" left stright parenthesis
        TOKEN_RSPAREN,     // "]" right straight parenthesis
        TOKEN_EXCLAMATION, // "!" exclamation mark
        TOKEN_ATSIGN,      // "@" at sign
        TOKEN_HASHTAG,     // "#" hashtag
        TOKEN_DOLLAR,      // "$" dollar sign
        TOKEN_PERCENT,     // "%" persent
        TOKEN_CARET,       // "^" caret also known as upward arrow
        TOKEN_AMPERSAND,   // "&" ampersand
        TOKEN_QUESTION,    // "?" question mark
        TOKEN_TILDE,       // "~" tilde
        TOKEN_SQUOTE,      // ' single quotation mark
        TOKEN_DQUOTE,      // " double quotation mark
        TOKEN_EOF,         // End of file
        TOKEN_UNKNOWN      // unknown
} tokenType;

// Holds the token
typedef struct {
        tokenType type;
        char *value; // stores the token as string
} token;

/*
// Keeping track of reading
typedef struct {
  char *source;  // Whole .pi file
  size_t length; // the file length
  size_t cursor; // current character position
} lexer;
*/

// functions
token **lexer_tokenizer(FILE *buffer);

#endif // !LEXER_H
