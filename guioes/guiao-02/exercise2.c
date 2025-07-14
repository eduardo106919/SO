#include <unistd.h>
#include <stdio.h>

int main(void) {
    pid_t identifier = fork();

    if (identifier == 0) {
        // child process
        printf("[C] process id: %d\n", getpid());
        // this value may change, if the parent exits before this instruction
        printf("[C] parent id:  %d\n", getppid());
        _exit(0);
    } else if (identifier > 0) {
        // parent process
        printf("[P] process id: %d\n", getpid());
        printf("[P] parent id:  %d\n", getppid());
        printf("[P] child id:   %d\n", identifier);
    } else {
        // error handling
        perror("fork()");
        return 1;
    }

    return 0;
}