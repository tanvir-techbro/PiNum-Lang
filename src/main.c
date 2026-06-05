#include "../include/lexer.h"
#include "../include/mode.h"
#include "../include/version.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

        // Exits if user does not provide any file
        if (argc < 2) {
                fprintf(stderr, "No file provided...\n");
                fprintf(stderr, "Usage: %s <file>\n", argv[0]);
                fprintf(stderr, "--help for more info\n");
                exit(EXIT_FAILURE);
        }

        // Handle flags
        if (strrchr(argv[1], '.') == NULL) {
                if (strcmp(argv[1], "--help") == 0) {
                        printf("pinum version %s\n\n", PINUM_VERSION);
                        printf("Usage: pinum <file.pn>\n");
                        printf("Flags:\n");
                        printf("  %-20s\tCheck pinum version. `pinum --version` or `pinum -v`\n", "--version or -v");
                        printf("  %-20s\tUpdate pinum. `pinum --update` or 'pinum -u'\n", "--update or -u");
                        return EXIT_SUCCESS;
                } else if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
                        printf("PiNum-Lang version %s\n", PINUM_VERSION);
                        return EXIT_SUCCESS;
                } else if (strcmp(argv[1], "--update") == 0 || strcmp(argv[1], "-u") == 0) {
                        printf("Checking for updates...\n");
                        // running curl command to update the system
                        int result = system("((curl -sSL --fail https://raw.githubusercontent.com/tanvir-techbro/PiNum-Lang/main/install.sh || echo \"exit 1\") | bash)");

                        // checking if the update ran successfully
                        if (result == 0) {
                                printf("pinum up to date!\n");
                                return EXIT_SUCCESS;
                        } else {
                                fprintf(stderr, "update failed.\n");
                                return EXIT_FAILURE;
                        }
                } else {
                        fprintf(stderr, "Invalid flag: %s\n", argv[1]);
                        printf("--help for more info\n");
                        return EXIT_SUCCESS;
                }
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
