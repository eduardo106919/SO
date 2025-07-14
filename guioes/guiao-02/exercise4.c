#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_PROC 10

int main(void) {
    int i = 0;
    pid_t identifier;

    for (i = 0; i < MAX_PROC; i++) {
        identifier = fork();

        // child process
        if (identifier == 0) {
            printf("[%d] parent id: %d\n", getpid(), getppid());
            _exit(i);
        } else if (identifier == -1) {
            // fork() error
            perror("fork()");
            return 1;
        }
    }

    int status = 0;
    for (i = 0; i < MAX_PROC; i++) {
        // wait for the child process
        identifier = wait(&status);
        if (identifier == -1) {
            perror("wait()");
            return 1;
        }

        if (WIFEXITED(status) != 0) {
            printf("process %d exit code: %d\n", identifier, WEXITSTATUS(status));
        } else {
            printf("process %d did not exit\n", identifier);
        }
    }

    return 0;
}