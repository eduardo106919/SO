#include <readline/history.h>
#include <readline/readline.h>
#include <wordexp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void execute_command(const char *command) {
    wordexp_t temp;
    // tokenize the string
    wordexp(command, &temp, 0);
    int count = temp.we_wordc;
    char *tokens[count + 1];

    // clone the strings
    for (int i = 0; i < count; i++) {
        tokens[i] = strdup(temp.we_wordv[i]);
    }

    tokens[count] = NULL;
    wordfree(&temp);

    execvp(tokens[0], tokens);
    perror(tokens[0]);
}

int sosh(void) {
    char *line = NULL;
    pid_t child = 0;
    int status = 0, background = 0;

    while ((line = readline("[sosh]$ ")) != NULL && (strcmp(line, "exit") != 0)) {
        add_history(line);

        // remove the new line
        line[strcspn(line, "\n")] = '\0';

        // background check
        if (line[strlen(line) - 1] == '&') {
            background = 1;
            line[strlen(line) - 1] = '\0';
        }

        child = fork();

        switch (child) {
        case -1:
            /* error code */
            perror("fork()");

            return -1;
        case 0:
            /* child code */
            execute_command(line);
            perror("execute_command()");

            _exit(1);
        default:
            /* parent code */
            if (background == 1) {
                printf("[%d] running in the background\n", child);
                background = 0;
            } else if (waitpid(child, &status, 0) == -1) {
                perror("wait()");
                return -1;
            }

            break;
        }
    }

    return 0;
}

int main(void) {
    using_history();

    sosh();

    clear_history();

    return 0;
}