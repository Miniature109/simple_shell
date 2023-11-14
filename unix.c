#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

int main() {
    char input[MAX_INPUT_SIZE];
    size_t len;

    while (1) {
        display_prompt();

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            /* Handle end-of-file (Ctrl+D) */
            printf("\nExiting the shell. Goodbye!\n");
            break;
        }

        /* Remove trailing newline character */
        len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        /* Execute the command */
        execute_command(input);
    }

    return 0;
}

