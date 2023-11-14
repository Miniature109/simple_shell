#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;  /* Declare the external variable environ */

#define MAX_INPUT_SIZE 1024

void display_prompt() {
    printf("simple_shell> ");
}

void execute_command(char *command) {
    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Child process */
        execlp(command, command, (char *)NULL);

        /* If execlp fails */
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        /* Parent process */
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process terminated abnormally\n");
        }
    }
}

void print_environment() {
    char **env_var = environ;

    while (*env_var != NULL) {
        printf("%s\n", *env_var);
        env_var++;
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    size_t len;

    while (1) {
        display_prompt();

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            /* Handle end-of-file (Ctrl+D) or other errors */
            printf("\nExiting the shell. Goodbye!\n");
            break;
        }

        /* Remove trailing newline character */
        len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        if (strcmp(input, "exit") == 0) {
            printf("Exiting the shell. Goodbye!\n");
            break;
        } else if (strcmp(input, "env") == 0) {
            print_environment();
        } else {
            /* Execute the command */
            execute_command(input);
        }
    }

    return 0;
}

