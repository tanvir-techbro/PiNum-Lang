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

#include "../include/ast.h"
#include "../include/error.h"
#include "../include/lexer.h"
#include "../include/mode.h"
#include "../include/parser.h"
#include "../include/version.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * PIPLELINE: check if the files provided are right -> tokenize line by line ->
 *            put tokens in a single line in a token_list -> pass the token_list to parser ->
 *            turn the list into a ASTnode (syntax tree) -> pass the ast to parser to be parsed ->
 *            check the codegen mode (engine or normal) -> pass the parssed nodes (normal codegen or engine codegen) to be compiled into C ->
 *            generate payload.c -> compile payload.c to payload (linux)/ payload.bin (for engine mode)
 *
 *            file.pn (input) -> src/main.c -> src/lexer.c src/lexer_filter.c src/helper.c -> src/main.c -> src/parser.c src/helper.c ->
 *            src/ast.c -> src/parser.c -> src/main.c -> src/codegen.c = payload/payload.bin (output)
 */

// - Flag handling functions -
void handle_flag_help();
void handle_version_flag();
// Updating pipeline
// returns 1 if an update is available, 0 if up to date; exits on failure.
static int check_update();
// downloads the installer and its checksum, then verifies the hash; exits on any failure.
static void check_hash(); // verify the installer script hash against release hash (install.sh.sha256)
// the parameter 'binray' is the file path to executeable program file.
// 'dir' is the working directory the command should run in (NULL = inherit current).
int run_cmd(const char *dir, const char *binary, char *const args[]); // helper function to run shell commands safely.
int handle_update_flag();

// --- MAIN ---
int main(int argc, char *argv[]) {
        // enable debug functions
        // --debug-all enables both options below
        bool debug_lexer = false; // enabled by --debug-lexer flag
        bool debug_ast = false;   // enabled by --debug-ast flag
        int arg_indx = 1;         // argument index. arg index 0 is the program binary name

        // Exits if user does not provide any file
        if (argc < 2) {
                fprintf(stderr, "Usage: %s <file>\n", argv[0]);
                fprintf(stderr, "See '--help' for more info.\n");
                pinum_error(STAGE_FILE, ERR_NO_INPUT_FILE, NULL);
        }

        // --- FLAG HANDLING (function calls) ---
        // the line below checks if the 2nd argument has any '.' in it.
        // if it has a '.' then it is a file, else it is a flag/option.
        if (strrchr(argv[arg_indx], '.') == NULL) {
                if (strcmp(argv[1], "--help") == 0) {
                        handle_flag_help();
                        return EXIT_SUCCESS;
                } else if (strcmp(argv[arg_indx], "--debug-all") == 0) {
                        debug_lexer = true;
                        debug_ast = true;
                        arg_indx++;
                } else if (strcmp(argv[arg_indx], "--debug-lexer") == 0) {
                        debug_lexer = true;
                        arg_indx++;
                } else if (strcmp(argv[arg_indx], "--debug-ast") == 0) {
                        debug_ast = true;
                        arg_indx++;
                } else if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
                        handle_version_flag();
                        return EXIT_SUCCESS;
                } else if (strcmp(argv[1], "--update") == 0 || strcmp(argv[1], "-u") == 0) {
                        int result = handle_update_flag();

                        // the update flag handles its own output and messaging
                        return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
                }
                // Unrecognized and invalid flag handling
                else {
                        printf("See '--help' for more info.\n");
                        pinum_error(STAGE_FILE, ERR_INVALID_FLAG, argv[1]);
                }
        }
        // --------------------------------------

        // --- FILE HANDLING ---
        char *filename = argv[arg_indx];
        char *extention = strrchr(argv[arg_indx], '.');
        FILE *buffer;

        // Checking if the file extention is valid or not.
        if (extention == NULL) {
                pinum_error(STAGE_FILE, ERR_INVALID_FILE_TYPE, NULL);
        } else if (!(strcmp(extention, ".pn"))) {
                // checking if the file can be opened or not
                if ((buffer = fopen(filename, "r")) == NULL) {
                        pinum_error(STAGE_FILE, ERR_CANNOT_OPEN_FILE, filename);
                }
                // tell the error reporter which file compile errors refer to
                error_set_source_file(filename);
                // If the file open is succesful it will continue with rest of the program.
        } else {
                pinum_error(STAGE_FILE, ERR_INVALID_FILE_TYPE, NULL);
        }
        // ---------------------

        // --- MAIN ---
        // Running the loop till we hit EOF (End Of File).
        token_list list;
        token_list_init(&list);
        token tokens = lexer_tokenizer(buffer);
        while (tokens.type != TOKEN_EOF) {
                // checking the tokens for specific types before adding it to the list
                if (tokens.type == TOKEN_HASHTAG) {
                        tokens = token_ignore_comment(tokens, buffer);
                        if (tokens.type == TOKEN_NLINE) {
                                token_list_add(&list, tokens);
                                if (debug_lexer) {
                                        // NOTE: this function call is for debugging purposes.
                                        lexer_print_token(tokens);
                                }
                                // Get next token for the next line
                                tokens = lexer_tokenizer(buffer);
                        }
                        continue;
                }

                token_list_add(&list, tokens);
                if (debug_lexer) {
                        // NOTE: this function call is for debugging purposes.
                        lexer_print_token(tokens);
                }
                // Update tokens for the next iteration
                tokens = lexer_tokenizer(buffer);
        }
        token_list_add(&list, tokens);

        // checking program mode if ENGINE_MODE is not enabled
        if (!ENGINE_MODE) check_program_mode(&list);

        ASTnode *ast = parse(&list);
        if (debug_ast) {
                // NOTE: this function call is for debugging purposes.
                print_ast(ast, 0);
        }
        free_ast_node(ast);
        // freeing the list and its tokens' values
        token_list_free(&list);

        // NOTE: 2 if statement below are and for debugging purposes.
        /*
        if (ENGINE_MODE) {
                printf("Enabled.\n");
        } else {
                printf("Disabled.\n");
        }
        */
        // ------------

        // --- CLEAN UP AND FINALIZATION ---
        // closing the file buffer
        fclose(buffer);
        return EXIT_SUCCESS;
        // ---------------------------------
}

// --- FLAG HANDLING (functions) ---
// handle the flag '--help'
void handle_flag_help() {
        printf("pinum version %s\n\n", PINUM_VERSION);
        printf("Usage: pinum [Flags] <file.pn>\n");
        printf("Flags:\n");
        printf("  %-20s\tEnable all debugging functions.\n", "--debug-all");
        printf("  %-20s\tEnable debugging functions for lexer.\n", "--debug-lexer");
        printf("  %-20s\tEnable debugging functions for ast.\n", "--debug-ast");
        printf("\n");
        printf("  %-20s\tDisplay pinum version information.\n", "--version or -v");
        printf("  %-20s\tUpdate pinum to the latest version.\n", "--update or -u");
        printf("  %-20s\tDisplay this output.\n", "--help or -h");
        printf("\nIf you find any issue, create a github issue at <https://github.com/tanvir-techbro/PiNum-Lang>\n");
}

// handle '--version' and '-v' flag
void handle_version_flag() {
        printf("PiNum-Lang version %s\n", PINUM_VERSION);
}

// Updating pipeline
static char g_latest_version[64] = {0};
static int check_update() {
        FILE *online_version = popen("curl -sSL --fail https://raw.githubusercontent.com/tanvir-techbro/PiNum-Lang/main/VERSION", "r");
        if (online_version == NULL) {
                pinum_error(STAGE_UPDATER, ERR_UPDATE_START, NULL);
        }

        char version[64] = {0};
        if (fgets(version, sizeof(version), online_version) != NULL) {
                // remove trailing newlines if there are
                version[strcspn(version, "\r\n")] = '\0';
        }
        snprintf(g_latest_version, sizeof(g_latest_version), "%s", version);

        int status = pclose(online_version);
        if (status != 0 || version[0] == '\0') {
                pinum_error(STAGE_UPDATER, ERR_UPDATE_CHECK, NULL); // fetch failed
        }
        // if version != PINUM_VERSION it returns 1 (update available),
        // if version == PINUM_VERSION it returns 0 (up to date)
        return strcmp(version, PINUM_VERSION) != 0;
}
static char g_update_dir[512] = {0};
// removes the two temp files and the temp directory used by the updater
static void cleanup_temp(void) {
        if (g_update_dir[0] == '\0') {
                return;
        }
        char installer_path[1024];
        char hash_path[1024];
        snprintf(installer_path, sizeof(installer_path), "%s/install.sh", g_update_dir);
        snprintf(hash_path, sizeof(hash_path), "%s/install.sh.sha256", g_update_dir);
        remove(installer_path);
        remove(hash_path);
        rmdir(g_update_dir);
        g_update_dir[0] = '\0';
}
static void check_hash() {
        // creating a unique directory
        char dir_template[] = "/tmp/pinum_update_XXXXXX";
        char *tmp_dir = mkdtemp(dir_template);
        if (!tmp_dir) {
                pinum_error(STAGE_UPDATER, ERR_UPDATE_TMP_DIR, NULL);
        }
        strncpy(g_update_dir, tmp_dir, sizeof(g_update_dir) - 1);

        // absolute file paths in the temp directory
        char installer_path[1024];
        char hash_path[1024];
        snprintf(installer_path, sizeof(installer_path), "%s/install.sh", g_update_dir);
        snprintf(hash_path, sizeof(hash_path), "%s/install.sh.sha256", g_update_dir);

        // script url, pinned to the same release tag as the checksum
        char script_url[1024] = {0};
        snprintf(script_url, sizeof(script_url), "https://raw.githubusercontent.com/tanvir-techbro/PiNum-Lang/v%s/install.sh", g_latest_version);

        // downloading install.sh into the installer_path
        printf("Downloading installer script (install.sh)...\n");
        char *curl_installer_args[] = {
            "/usr/bin/curl",
            "-sSL",
            script_url,
            "-o",
            installer_path,
            NULL,
        };
        // curl installation failed
        if (run_cmd(NULL, "/usr/bin/curl", curl_installer_args) != 0) {
                cleanup_temp();
                pinum_error(STAGE_UPDATER, ERR_UPDATE_DOWNLOAD_SCRIPT, NULL);
        }

        // hash url
        char hash_url[1024] = {0};
        snprintf(hash_url, sizeof(hash_url), "https://github.com/tanvir-techbro/PiNum-Lang/releases/download/v%s/install.sh.sha256", g_latest_version);

        // downloading install.sh.sha256 into the hash_path
        printf("Downloading checksum file (install.sh.sha256)...\n");
        char *curl_hash_args[] = {
            "/usr/bin/curl",
            "-sSL",
            hash_url,
            "-o",
            hash_path,
            NULL,
        };
        // curl installation failed
        if (run_cmd(NULL, "/usr/bin/curl", curl_hash_args) != 0) {
                cleanup_temp();
                pinum_error(STAGE_UPDATER, ERR_UPDATE_DOWNLOAD_CHECKSUM, NULL);
        }

        // verify the checksum in the temp directory so the relative
        // "install.sh" listed in the checksum file resolves correctly.
        char *sha_args[] = {
            "/usr/bin/sha256sum",
            "--check",
            "--status",
            "install.sh.sha256",
            NULL,
        };
        if (run_cmd(g_update_dir, "/usr/bin/sha256sum", sha_args) != 0) {
                cleanup_temp();
                pinum_error(STAGE_UPDATER, ERR_UPDATE_CHECKSUM_VERIFY, NULL);
        }
        // checksum passed; keep files for install step
}
int run_cmd(const char *dir, const char *binary, char *const args[]) {
        // fork the process
        pid_t pid = fork();

        // child process
        if (pid == 0) {
                // change into the requested working directory, if any
                if (dir != NULL && chdir(dir) < 0) {
                        pinum_error(STAGE_UPDATER, ERR_UPDATE_CHDIR, strerror(errno));
                }
                // replace the child process binray with target binray
                execv(binary, args);
                // if execv reach this line, it failed
                pinum_error(STAGE_UPDATER, ERR_UPDATE_EXEC, strerror(errno));
        }
        // parent process
        else if (pid > 0) {
                int status;
                // pause and wait for the child process to finish
                waitpid(pid, &status, 0);
                // check if child exited with clean code 0
                return (WIFEXITED(status) && WEXITSTATUS(status) == 0) ? 0 : 1;
        }

        // fork entirely failed
        return -1;
}
// handle '--update' and '-u' flag
int handle_update_flag() {
        printf("Checking for updates...\n");
        int check = check_update();
        if (check == 0) {
                // pinum up to date
                printf("Up to date!\n");
                return EXIT_SUCCESS;
        }
        // downloads and verifies the installer; exits on any failure
        check_hash();

        printf("Verification successful!\n");
        printf("\nExecuting installer...\n");
        char *bash_args[] = {
            "/bin/bash",
            "install.sh",
            NULL,
        };
        int result = run_cmd(g_update_dir, "/bin/bash", bash_args);

        cleanup_temp();

        return result == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
// ---------------------------------
