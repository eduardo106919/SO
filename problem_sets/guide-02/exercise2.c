#include <unistd.h>
#include <stdio.h>

int main(void) {
    pid_t identifier = fork();
    int status = 0;

    if (identifier == 0) {
        printf("c | id: %d\n", getpid());
        printf("c | parent id: %d\n", getppid());
        _exit(0);
    } else if (identifier > 0) {
        // wait for the child
        wait(&status);

        printf("p | id: %d\n", getpid());
        printf("p | parent id: %d\n", getppid());
        printf("p | child id: %d\n", identifier);
    } else {
        // error handling
        perror("fork()");
        return 1;
    }

    return 0;
}