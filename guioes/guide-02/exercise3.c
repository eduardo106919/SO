#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

#define MAX_PROC 10

int main(void) {
    int exit_codes[MAX_PROC];
    pid_t children_ids[MAX_PROC];

    int count = 0, status = 0;
    pid_t id;

    while (count < MAX_PROC) {
        id = fork();
        
        // child process
        if (id == 0) {
            printf("%2d | id     = %d\n", count + 1, getpid());
            printf("%2d | parent = %d\n\n", count + 1, getppid());

            _exit(count + 1);
        } else if (id < 0) {
            // error handling
            perror("fork");
            return 1;
        }

        // wait for the child process to finish
        children_ids[count] = wait(&status);
        if (WIFEXITED(status) == 1)
            exit_codes[count] = WEXITSTATUS(status);

        count++;
    }

    count = 0;
    while (count < MAX_PROC) {
        printf("Exit code of %d: %2d\n", children_ids[count], exit_codes[count]);
        count++;
    }

    return 0;
}