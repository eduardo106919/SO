#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/wait.h>


int main(void) {
    char *command1[] = {"grep", "-v", "^#", "/etc/passwd", NULL};
    char *command2[] = {"cut", "-f7", "-d:", NULL};
    char *command3[] = {"uniq", NULL};
    char *command4[] = {"wc", "-l", NULL};
    
    int fildes[3][2];

    if (pipe(fildes[0]) == -1) {
        perror("pipe()");
        return 1;
    }

    pid_t child = fork();

    // grep -v ^# /etc/passwd
    if (child == 0) {
        // child process

        close(fildes[0][0]); // close reading side of the pipe

        dup2(fildes[0][1], 1); // redirect stdout to the writing side of the pipe
        close(fildes[0][1]);

        execvp(command1[0], command1);
        perror(command1[0]);
        _exit(127);
    } else if (child == -1) {
        // error code
        close(fildes[0][0]);
        close(fildes[0][1]);
        return 1;
    }

    close(fildes[0][1]); // close writing side of the pipe

    if (pipe(fildes[1]) == -1) {
        perror("pipe()");
        close(fildes[0][0]);
        return 1;
    }


    child = fork();

    // grep -v ^# /etc/passwd | cut -f7 -d:
    if (child == 0) {
        // child process

        close(fildes[1][0]); // close reading side of the pipe

        dup2(fildes[0][0], 0); // redirect stdin to the reading side of the pipe
        close(fildes[0][0]);

        dup2(fildes[1][1], 1); // redirect stdout to the writing side of the pipe
        close(fildes[1][1]);

        execvp(command2[0], command2);
        perror(command2[0]);
        _exit(127);
    } else if (child == -1) {
        // error code
        close(fildes[0][0]);
        close(fildes[0][1]);
        return 1;
    }

    close(fildes[0][0]); // close reading side of the pipe
    close(fildes[1][1]); // close writing side of the pipe

    if (pipe(fildes[2]) == -1) {
        perror("pipe()");
        close(fildes[1][0]);
        return 1;
    }

    child = fork();

    // grep -v ^# /etc/passwd | cut -f7 -d: | uniq
    if (child == 0) {
        // child process

        close(fildes[2][0]); // close reading side of the pipe

        dup2(fildes[1][0], 0); // redirect stdin to the reading side of the pipe
        close(fildes[1][0]);

        dup2(fildes[2][1], 1); // redirect stdout to the writing side of the pipe
        close(fildes[2][1]);

        execvp(command3[0], command3);
        perror(command3[0]);
        _exit(127);
    } else if (child == -1) {
        // error code
        close(fildes[0][0]);
        close(fildes[0][1]);
        return 1;
    }

    close(fildes[1][0]);
    close(fildes[2][1]);

    child = fork();

    // grep -v ^# /etc/passwd | cut -f7 -d: | uniq | wc -l
    if (child == 0) {
        // child process

        dup2(fildes[2][0], 0); // redirect stdin to the reading side of the pipe
        close(fildes[2][0]);

        execvp(command4[0], command4);
        perror(command4[0]);
        _exit(127);
    } else if (child == -1) {
        // error code
        close(fildes[0][0]);
        close(fildes[0][1]);
        return 1;
    }

    // wait for all the processes
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    return 0;
}
