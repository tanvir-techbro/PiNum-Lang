#include "../include/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int tests_run = 0;
static int tests_passed = 0;

#define assert_eq(actual, expected, msg)                                                       \
        do {                                                                                   \
                tests_run++;                                                                   \
                if ((actual) == (expected)) {                                                  \
                        tests_passed++;                                                        \
                } else {                                                                       \
                        printf("FAIL: %s (expected %d, got %d)\n", msg, (expected), (actual)); \
                }                                                                              \
        } while (0)

#define assert_str_eq(actual, expected, msg)                                                                         \
        do {                                                                                                         \
                tests_run++;                                                                                         \
                if ((actual) != NULL && strcmp((actual), (expected)) == 0) {                                         \
                        tests_passed++;                                                                              \
                } else {                                                                                             \
                        printf("FAIL: %s (expected '%s', got '%s')\n", msg, (expected), (actual) ? actual : "NULL"); \
                }                                                                                                    \
        } while (0)

void test_lexer_tokenize_words_keywords() {
        const char *keywords[] = {"int", "float", "print", "if", "else", "return",
                                  "import", "read", "long", "short", "signed",
                                  "unsigned", "bool", "double", "char", "string",
                                  "true", "false", "stdlib", "math"};
        tokenType expected_types[] = {
            TOKEN_INT, TOKEN_FLOAT, TOKEN_PRINT, TOKEN_IF, TOKEN_ELSE,
            TOKEN_RETURN, TOKEN_IMPORT, TOKEN_READ, TOKEN_LONG, TOKEN_SHORT,
            TOKEN_SIGNED, TOKEN_UNSIGNED, TOKEN_BOOL, TOKEN_DOUBLE, TOKEN_CHAR,
            TOKEN_STRING, TOKEN_TRUE, TOKEN_FALSE, TOKEN_LIB_STDLIB, TOKEN_LIB_MATH};

        for (int i = 0; i < 20; i++) {
                FILE *f = fmemopen((void *)keywords[i], strlen(keywords[i]), "r");
                token t = lexer_tokenize_words(f);
                assert_eq(t.type, expected_types[i], keywords[i]);
                fclose(f);
        }
}

void test_lexer_tokenize_words_identifier() {
        const char *identifiers[] = {"x", "myVar", "foo_bar", "var123", "_private"};

        for (int i = 0; i < 5; i++) {
                FILE *f = fmemopen((void *)identifiers[i], strlen(identifiers[i]), "r");
                token t = lexer_tokenize_words(f);
                assert_eq(t.type, TOKEN_ID, identifiers[i]);
                assert_str_eq(t.value, identifiers[i], identifiers[i]);
                free(t.value);
                fclose(f);
        }
}

void test_lexer_tokenize_numbers_integers() {
        const char *numbers[] = {"0", "42", "12345", "999"};
        int expected[] = {0, 42, 12345, 999};

        for (int i = 0; i < 4; i++) {
                FILE *f = fmemopen((void *)numbers[i], strlen(numbers[i]), "r");
                token t = lexer_tokenize_numbers(f);
                assert_eq(t.type, TOKEN_INUM, numbers[i]);
                assert_eq(t.int_value, expected[i], numbers[i]);
                fclose(f);
        }
}

void test_lexer_tokenize_numbers_floats() {
        const char *numbers[] = {"0.5", "3.14", "123.456", "0.0"};
        double expected[] = {0.5, 3.14, 123.456, 0.0};

        for (int i = 0; i < 4; i++) {
                FILE *f = fmemopen((void *)numbers[i], strlen(numbers[i]), "r");
                token t = lexer_tokenize_numbers(f);
                assert_eq(t.type, TOKEN_FNUM, numbers[i]);
                assert_eq(t.float_value, expected[i], numbers[i]);
                fclose(f);
        }
}

void test_lexer_tokenize_numbers_leading_dot() {
        const char *numbers[] = {".5", ".123"};
        double expected[] = {0.5, 0.123};

        for (int i = 0; i < 2; i++) {
                FILE *f = fmemopen((void *)numbers[i], strlen(numbers[i]), "r");
                token t = lexer_tokenize_numbers(f);
                assert_eq(t.type, TOKEN_FNUM, numbers[i]);
                assert_eq(t.float_value, expected[i], numbers[i]);
                fclose(f);
        }
}

int main() {
        printf("Running lexer tests...\n\n");

        test_lexer_tokenize_words_keywords();
        test_lexer_tokenize_words_identifier();
        test_lexer_tokenize_numbers_integers();
        test_lexer_tokenize_numbers_floats();
        test_lexer_tokenize_numbers_leading_dot();

        printf("\nResults: %d/%d tests passed\n", tests_passed, tests_run);
        return tests_passed == tests_run ? 0 : 1;
}