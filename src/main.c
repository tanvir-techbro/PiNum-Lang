/********************************************************************
 *   _____ _ _   _                       _                          *
 *  |  __ (_) \ | |                     | |                         *
 *  | |__) ||  \| |_   _ _ __ ___ ______| |     __ _ _ __   __ _    *
 *  |  ___/ | . ` | | | | '_ ` _ \______| |    / _` | '_ \ / _` |   *
 *  | |   | | |\  | |_| | | | | | |     | |___| (_| | | | | (_| |   *
 *  |_|   | |_| \_|\__,_|_| |_| |_|     |______\__,_|_| |_|\__, |   *
 *                                                          __/ |   *
 *                                                         |___/    *
 *                                                                  *
 *  Copyright (c) 2026 tanvir-techbro.                              *
 *  You may opt to use, copy, modify, merge, publish, distribute    *
 *  and/or sell copies of the Software, and permit persons to whom  *
 *  the Software is furnished to do so, under the conditions of the *
 *  LICENSE.                                                        *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, *
 *  EXPRESS OR IMPLIED.                                             *
 *                                                                  *
 *  If you find any bug you would be highly encouraged to create a  *
 *  github issue at <https://github.com/tanvir-techbro/PiNum-Lang>  *
 *  or contact <surjointelligence.team@gmail.com>                   *
 ********************************************************************/

#include "../include/lexer.h"
#include "../include/mode.h"
#include "../include/version.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// flag handling functions
void handle_flag_help();
void handle_version_flag();
int handle_update_flag();

// --- MAIN ---
int main(int argc, char *argv[]) {

        // Exits if user does not provide any file
        if (argc < 2) {
                // code '\033[1;40m' makes the compiler name text bolder and code `\033[0m` resets to default
                // code `\033[1;31m` makes the text 'fatal error' red and code `\033[0m` resets to default color
                fprintf(stderr, "\033[1;40m%s:\033[0m \033[1;31mfatal error:\033[0m no input file provided.\n", argv[0]);
                fprintf(stderr, "Usage: %s <file>\n", argv[0]);
                fprintf(stderr, "See '--help' for more info.\n");
                exit(EXIT_FAILURE);
        }

        // --- FLAG HANDLING (function calls) ---
        // the line below checks if the 2nd argument has any '.' in it.
        // if it has a '.' then it is a file, else it is a flag/option.
        if (strrchr(argv[1], '.') == NULL) {
                if (strcmp(argv[1], "--help") == 0) {
                        handle_flag_help();
                        return EXIT_SUCCESS;
                } else if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
                        handle_version_flag();
                        return EXIT_SUCCESS;
                } else if (strcmp(argv[1], "--update") == 0 || strcmp(argv[1], "-u") == 0) {
                        int result = handle_update_flag();

                        // checking if the update ran successfully
                        if (result == 0) {
                                printf("pinum up to date!\n");
                                return EXIT_SUCCESS;
                        } else {
                                fprintf(stderr, "update failed.\n");
                                return EXIT_FAILURE;
                        }

                }
                // Unrecognized and invalid flag handling
                else {
                        // code '\033[1;40m' makes the compiler name text bolder and code `\033[0m` resets to default
                        // code `\033[1;31m` makes the text 'fatal error' red and code `\033[0m` resets to default color
                        fprintf(stderr, "\033[1;40m%s:\033[0m \033[1;31merror:\033[0m invalid flag '%s'\n", argv[0], argv[1]);
                        printf("See '--help' for more info.\n");
                        return EXIT_SUCCESS;
                }
        }
        // --------------------------------------

        // --- FILE HANDLING ---
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
        // ---------------------

        // --- MAIN ---
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
        // ------------

        // --- CLEAN UP AND FINALIZATION ---
        // Clean up final EOF token value
        if (tokens.value) {
                free(tokens.value);
        }

        // closing the file buffer
        fclose(buffer);
        return EXIT_SUCCESS;
        // ---------------------------------
}

// --- FLAG HANDLING (functions) ---
// handle the flag '--help'
void handle_flag_help() {
        printf("pinum version %s\n\n", PINUM_VERSION);
        printf("Usage: pinum <file.pn>\n");
        printf("Flags:\n");
        printf("  %-20s\tDisplay pinum version information.\n", "--version or -v");
        printf("  %-20s\tUpdate pinum to the latest version.\n", "--update or -u");
        printf("\nIf you find any issue, create a github issue at <https://github.com/tanvir-techbro/PiNum-Lang>\n");
}
// handle '--version' and '-v' flag
void handle_version_flag() {
        printf("PiNum-Lang version %s\n", PINUM_VERSION);
}
// handle '--update' and '-u' flag
int handle_update_flag() {
        printf("Checking for updates...\n");
        // running curl command to update the system
        int result = system("((curl -sSL --fail https://raw.githubusercontent.com/tanvir-techbro/PiNum-Lang/main/install.sh || echo \"exit 1\") | bash)");

        return result;
}
// ---------------------------------
