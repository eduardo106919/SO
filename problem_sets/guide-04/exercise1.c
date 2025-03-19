#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int main(void) {
    int status = 0;
    pid_t child = 0;
    int fildes[2];
    char buffer[30];
    ssize_t len = 0;

    if (pipe(fildes) == -1) {
        /* pipe error */
        perror("pipe()");

        return 1;
    }

    child = fork();

    switch (child) {
        case -1:
            /* error code */
            perror("fork()");

            return 1;
        case 0:
            /* child code */
            close(fildes[1]);
            
            len = read(fildes[0], &buffer, sizeof(buffer));
            close(fildes[0]);
            
            if (len == -1) {
                perror("read()");

                _exit(1);
            }

            printf("process %d received '%s' from process %d\n", getpid(), buffer, getppid());
            _exit(0);
        default:
            /* parent code */
            close(fildes[0]);

            strcpy(buffer, "operating systems class");
            printf("process %d sending '%s' to process %d\n", getpid(), buffer, child);
            pid_t brother = fork();
            if (brother == -1) {
                /* error code */
                perror("fork()");

                return 1;
            } else if (brother == 0) {
                /* child code */
                execlp("sleep", "sleep", "5", NULL);
                perror("sleep");
                _exit(1);
            }

            /* wait for sleep to end */
            if (wait(&status) == -1) {
                perror("wait()");
                return 1;
            }

            /* get return value from the second child */
            if (WIFEXITED(status) != 0 && WEXITSTATUS(status) != 0) {
                printf("exec() error\n");
                return 1;
            }

            len = write(fildes[1], buffer, sizeof(buffer));
            close(fildes[1]);

            if (len == -1) {
                perror("write()");
                return 2;
            }

            break;
    }

    return 0;
}