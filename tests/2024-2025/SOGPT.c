
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>


// number of processes
#define PROCS 10


int main(int argc, char **argv) {

    int first[2], second[2];

    // pipe for filter -> execute
    pipe(first);

    /* filter */
    if (fork() == 0) {
        close(first[0]); // close reading side of the pipe

        // redirect stdout to the writing side of the pipe
        dup2(first[1], 1);
        close(first[1]);

        execlp("filter", "filter", argv[1], NULL);
        perror("filter");
        _exit(127);
    }

    close(first[1]);

    // pipe for execute -> merge
    pipe(second);
    ssize_t out = 0;

    // optimize execute with N processes
    for (int i = 0; i < PROCS; i++) {
        /* execute */
        if (fork() == 0) {

            close(second[0]); // close reading side of the pipe

            // redirect stdin to the reading side of the pipe
            dup2(first[0], 0);
            close(first[0]);
            
            // redirect stdout to the writing side of the pipe
            dup2(second[1], 1);
            close(second[1]);

            execlp("execute", "execute", argv[1], NULL);
            perror("execute");
            _exit(127);
        }
    }

    close(first[0]);
    close(second[1]);

    /* merge */
    if (fork() == 0) {
        // redirect stdin to the reading side of the pipe
        dup2(second[0], 0);
        close(second[0]);

        execlp("merge", "merge", argv[1], NULL);
        perror("merge");
        _exit(127);
    }

    close(second[0]);

    // wait for all the processes
    for (int i = 0; i < PROCS + 2; i++) {
        wait(NULL);
    }

    return 0;
}
