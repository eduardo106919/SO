#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

#define MAX_PROC 10

int main(void) {
    int count = 0, status = 0;
    pid_t id;

    while (count < MAX_PROC) {
        id = fork();
        
        if (id == 0) {
            // child process
            printf("%2d | process id: %d\n", count + 1, getpid());
            printf("%2d | parent id:  %d\n", count + 1, getppid());

            _exit(count + 1);
        } else if (id < 0) {
            // error handling
            perror("fork");
            return 1;
        }

        // wait for child process
        id = wait(&status);
        if (id == -1) {
            perror("wait()");
            return 1;
        }

        // wait for the child process to finish
        if (WIFEXITED(status) == 1) {
            printf("process %d exit code: %d\n\n", id, WEXITSTATUS(status));
        } else {
            printf("process %d did not exit\n\n", id);
        }

        count++;
    }

    return 0;
}