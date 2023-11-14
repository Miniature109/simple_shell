#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

int main() {
    char input[MAX_INPUT_SIZE];
    pid_t pid;
    char* path;
    char* path_copy;
    char* path_token;

    while (1) {
        /* Print the shell prompt */
        printf("Shell> ");
        
        /* Read user input */
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        /* Remove the newline character */
        input[strcspn(input, "\n")] = '\0';

        /* Exit the shell if the user enters 'exit' or 'quit' */
        if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
            break;
        }

        /* Fork a new process */
        pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            /* Child process */

            /* Parse the command and arguments */
            char* args[MAX_INPUT_SIZE];
            char* token = strtok(input, " ");
            int i = 0;

            while (token != NULL) {
                args[i] = token;
                token = strtok(NULL, " ");
                i++;
            }

            args[i] = NULL;

            /* Search for the command in the PATH */
            path = getenv("PATH");
            path_copy = strdup(path);
            path_token = strtok(path_copy, ":");

            while (path_token != NULL) {
                char full_path[MAX_INPUT_SIZE];
                snprintf(full_path, sizeof(full_path), "%s/%s", path_token, args[0]);

                if (access(full_path, X_OK) == 0) {
                    /* Execute the command */
                    execv(full_path, args);

                    /* If execv() fails, continue searching the PATH */
                }

                path_token = strtok(NULL, ":");
            }

            free(path_copy);

            /* If the loop completes, the command was not found */
            fprintf(stderr, "Command not found: %s\n", args[0]);
            exit(EXIT_FAILURE);
        } else {
            /* Parent process */
            int status;
            waitpid(pid, &status, 0);
        }
    }

    printf("Shell exited.\n");
    return 0;
}
