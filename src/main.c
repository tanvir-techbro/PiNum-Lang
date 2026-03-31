#include "../include/lexer.h"
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
        token tokens;
        while (true) {
                tokens = lexer_tokenizer(buffer);

                if (tokens.type == TOKEN_EOF) {
                        if (tokens.value) {
                                free(tokens.value);
                        }
                        break;
                }

                // freeing the value
                free(tokens.value);
        }

        // closing the file buffer
        fclose(buffer);
        return EXIT_SUCCESS;
}
