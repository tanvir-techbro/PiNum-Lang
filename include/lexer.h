#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// All the tokentypes
typedef enum {
        TOKEN_ID,   // normal tokens maybe strings and characters inside print function, also can be veriables.
        TOKEN_INUM, // intager numbers: 10, 50, 42 etc.
        TOKEN_FNUM, // floating point numbers: 3.14, 0.5 etc.
        // Libreries
        TOKEN_LIB_STDLIB, // if user does not include `@for engine` then user need to include `@import stdlib` to use standard librery features
        TOKEN_LIB_MATH,   // if user does not include `@for engine` then user need to include `@import stdlib` to use standard math features
        // Keywords
        TOKEN_IMPORT, // token import, can be used at the starting of the file after atsign to import files and libreries
        TOKEN_IF,     // kewword if
        TOKEN_ELSE,   // Keyword else
        TOKEN_PRINT,  // keyword print
        TOKEN_READ,   // keyword read, takes input from user
        TOKEN_RETURN, // keyword return
        // Data types
        TOKEN_LONG,     // long prefix
        TOKEN_SHORT,    // short prefix
        TOKEN_SIGNED,   // signed prefix
        TOKEN_UNSIGNED, // unsigned prefix
        TOKEN_BOOL,     // boolean data type
        TOKEN_INT,      // int datatype
        TOKEN_FLOAT,    // float datatype
        TOKEN_DOUBLE,   // double datatype
        TOKEN_CHAR,     // char datatype
        TOKEN_STRING,   // string datatype, basiclly char array in the transpiled code
        TOKEN_TRUE,     // boolean literal true
        TOKEN_FALSE,    // boolean literal false
        // single character tokens
        TOKEN_DOT,         // "." dot
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
        TOKEN_LABRACKET,   // "<" left angle bracket
        TOKEN_RABRACKET,   // ">" right angle bracket
        TOKEN_EXCLAMATION, // "!" exclamation mark
        TOKEN_ATSIGN,      // "@" at sign
        TOKEN_HASHTAG,     // "#" hashtag
        TOKEN_DOLLAR,      // "$" dollar sign
        TOKEN_PERCENT,     // "%" persent
        TOKEN_CARET,       // "^" caret also known as upward arrow
        TOKEN_AMPERSAND,   // "&" ampersand
        TOKEN_AND,         // "&&" and operator
        TOKEN_PIPE,        // "|" pipe
        TOKEN_OR,          // "||" or operator
        TOKEN_QUESTION,    // "?" question mark
        TOKEN_TILDE,       // "~" tilde
        TOKEN_SQUOTE,      // ' single quotation mark
        TOKEN_DQUOTE,      // " double quotation mark
        // Multi-character operators
        TOKEN_EEQUAL, // "==" double equal
        TOKEN_NEQUAL, // "!=" not equal
        TOKEN_LEQUAL, // "<=" less than or equal
        TOKEN_GEQUAL, // ">=" greater than or equal
        // special character tokens
        TOKEN_NTERMINATOR, // '\0' null terminator
        TOKEN_NLINE,       // '\n' newline character
        TOKEN_TAB,         // '\t' tab
        // EOF and unknown
        TOKEN_EOF,    // End of file
        TOKEN_UNKNOWN // unknown
} tokenType;

// Holds the token
typedef struct {
        tokenType type;
        char *value;        // stores the token as string
        int int_value;      // stores the token as intager value
        double float_value; // stores the token as float value
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
token lexer_tokenizer(FILE *buffer);
token lexer_tokenize_numbers(FILE *buffer);
token lexer_tokenize_words(FILE *buffer);
// NOTE: these two functions are for debugging purposes and temporary.
const char *lexer_token_type_to_string(tokenType type);
void lexer_print_token(token t);

#endif // !LEXER_H
