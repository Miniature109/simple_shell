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
            char* args[MAX_INPUT_SIZE];
            char* token = strtok(input, " ");
            int i = 0;

            while (token != NULL) {
                args[i] = token;
                token = strtok(NULL, " ");
                i++;
            }

            args[i] = NULL;

            if (execvp(args[0], args) == -1) {
                perror("Execution failed");
                exit(EXIT_FAILURE);
            }
        } else {
            /* Parent process */
            int status;
            waitpid(pid, &status, 0);
        }
    }

    printf("Shell exited.\n");
    return 0;
}
