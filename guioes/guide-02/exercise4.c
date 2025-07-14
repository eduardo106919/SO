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
            printf("parent %d | process %d\n", getppid(), getpid());
            _exit(i);
        } else if (identifier == -1) {
            // fork() error
            perror("fork()");
            return 1;
        }
    }

    int status = 0, out = 0;
    for (i = 0; i < MAX_PROC; i++) {
        identifier = wait(&status);

        if (identifier > 0 && WIFEXITED(status) != 0) {
            out = WEXITSTATUS(status);
            printf("-> process %d exited with %d\n", identifier, out);
        } else if (identifier == -1) {
            // wait() error
            perror("wait()");
            return 1;
        }
    }

    return 0;
}