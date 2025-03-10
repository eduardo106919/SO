#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void) {

    pid_t child = fork();
    if (child == -1) {
        perror("fork()");
        return 1;
    } else if (child == 0) {
        // child process
        printf("process %d executing 'ls -l'\n", getpid());
        execlp("ls", "l", "-l", NULL);

        // execl() got an error
        _exit(1);
    } else {
        // parent process
        int status = 0;
        child = wait(&status);
        if (child == -1) {
            perror("wait()");
            return 1;
        }

        // execl() got an error
        if (status == 1) {
            printf("something went wrong with process %d!\n", child);
        } else {
            printf("process %d executed with sucess!\n", child);
        }
    }


    return 0;
}