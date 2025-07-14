#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

/**
 * @brief Executes the command multiple times, until the return value is 0
 * 
 * @param command command to execute
 * @return number of times the command was executed
 */
int multiple_executions(const char *command) {
    int res = -1;

    if (command != NULL) {
        int count = 0, out = -1, status = 0;
        pid_t child = 0;

        while (out != 0) {
            child = fork();

            switch (child) {
            case -1:
                // error code
                perror("fork()");

                return -1;
            case 0:
                // child code

                execlp(command, command, NULL);

                perror(command);
                _exit(127);
            default:
                // parent code
                child = waitpid(child, &status, 0);

                if (WIFEXITED(status) != 0) {
                    out = WEXITSTATUS(status);
                    count++;
                }

                break;
            }
        }

        res = count;
    }

    return res;
}

int * controlador(char **commands, int argc) {
    if (commands != NULL) {
        pid_t child = 0;
        int out = 1, status = 0, count = 0;
        pid_t children[argc];

        int i;
        for (i = 0; i < argc; i++) {
            children[i] = fork();

            if (children[i] == -1) {
                perror("fork()");

                return NULL;
            } else if (children[i] == 0) {

                // execute the command as many times it needs
                count = multiple_executions(commands[i]);

                if (count == -1)
                    _exit(0);

                _exit(count);
            }
        }

        int *result = malloc(sizeof(int) * argc);
        if (result == NULL) {
            perror("malloc()");

            return NULL;
        }

        memset(result, 0, sizeof(int) * argc);

        for (i = 0; i < argc; i++) {
            child = waitpid(children[i], &status, 0);

            if (child != -1 && WIFEXITED(status) != 0) {
                out = WEXITSTATUS(status);
                if (out == 0) {
                    printf("something went wrong with process %d\n", children[i]);

                    return NULL;
                } else {
                    result[i] = out;
                }
            }
        }

        return result;
    }

    return NULL;
}



int main(int argc, char **argv) {

    int * count = controlador(argv + 1, argc - 1);

    /*
    WARNING:
        the values that are shown may not be acurate,
        because _exit() system call only allows integers
        between 0-255, so the count will be truncated to those values
    */

    for (int i = 1; i < argc; i++) {
        printf("%s %d\n", argv[i], count[i - 1]);
    }

    return 0;
}