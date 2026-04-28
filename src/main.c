#include "../include/lexer.h"
#include "../include/mode.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

        // Exits if user does not provide any file
        if (argc < 2) {
                fprintf(stderr, "No file provided...\n");
                fprintf(stderr, "Usage: %s <file>\n", argv[0]);
                exit(EXIT_FAILURE);
        }

        char *filename = argv[1];
        char *extention = strrchr(argv[1], '.');
        FILE *buffer;

        // Checking if the file extention is valid or not.
        if (extention == NULL) {
                fprintf(stderr, "Filetype not valid...\n");
                exit(EXIT_FAILURE);
        } else if (!(strcmp(extention, ".pn"))) {

                // checking if the file can be opened or not
                if ((buffer = fopen(filename, "r")) == NULL) {
                        perror("Error opening file...\n");
                        exit(EXIT_FAILURE);
                }
                // If the file open is succesful it will continue with rest of the program.

        } else {
                fprintf(stderr, "Filetype not valid...\n");
                exit(EXIT_FAILURE);
        }

        // Running the loop till we hit EOF (End Of File).
        token tokens = lexer_tokenizer(buffer);
        while (tokens.type != TOKEN_EOF) {
                token_list list;
                token_list_init(&list);

                while (tokens.type != TOKEN_NLINE && tokens.type != TOKEN_EOF) {
                        // checking the tokens for specific types before adding it to the list
                        if (tokens.type == TOKEN_HASHTAG) {
                                tokens = token_ignore_comment(tokens, buffer);
                                if (tokens.type == TOKEN_NLINE || tokens.type == TOKEN_EOF) {
                                        break;
                                }
                        }
                        // Checking for quoted text, and turnig them into TOKEN_ID
                        else if (tokens.type == TOKEN_SQUOTE || tokens.type == TOKEN_DQUOTE) {
                                tokens = token_quote_handler(tokens, buffer);
                                if (tokens.type == TOKEN_NLINE || tokens.type == TOKEN_EOF) {
                                        break;
                                }
                                token_list_add(&list, tokens);
                        }
                        // just normally add it to the list
                        else {
                                token_list_add(&list, tokens);
                        }

                        // NOTE: this function call is temporary for debugging purposes.
                        lexer_print_token(tokens);

                        // Update tokens for the next iteration
                        tokens = lexer_tokenizer(buffer);
                }

                // If we stopped at a newline, we might want to add it to the list or just skip it
                if (tokens.type == TOKEN_NLINE) {
                        token_list_add(&list, tokens);
                        lexer_print_token(tokens);
                        // Get next token for the next line
                        tokens = lexer_tokenizer(buffer);
                }

                // NOTE: 2 if statement below are temporary and for debugging purposes.
                if (ENGINE_MODE) {
                        printf("Enabled.\n");
                } else {
                        printf("Disabled.\n");
                }

                // checking program mode if ENGINE_MODE is not enabled
                if (!ENGINE_MODE) {
                        check_program_mode(&list);
                }
                // freeing the list and its tokens' values
                token_list_free(&list);
        }

        // Clean up final EOF token value
        if (tokens.value) {
                free(tokens.value);
        }

        // closing the file buffer
        fclose(buffer);
        return EXIT_SUCCESS;
}
