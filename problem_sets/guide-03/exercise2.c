#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void) {

    int res = 0;
    pid_t child = fork();
    if (child == -1) {
        perror("fork()");
        return 1;
    } else if (child == 0) {
        // child process
        printf("process %d executing 'ls -l'\n", getpid());
        execlp("ls", "l", "-l", NULL);

        perror("ls");
        // execl() got an error
        _exit(127);
    } else {
        // parent process
        int status = 0;
        child = wait(&status);
        if (child == -1) {
            perror("wait()");
            return 1;
        }

        if (WIFEXITED(status) != 0) {
            // exec() went wrong
            res = WEXITSTATUS(status);
                
            if (res == 127) {
                printf("Something went wrong with process %d\n", child);
            } else {
                printf("Process %d returned %d\n", child, res);
            }
        }
    }

    return 0;
}