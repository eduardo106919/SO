#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

int main(void) {

    pid_t proc = 0;
    ssize_t len = 0;
    int fildes[2];
    char buffer[32];
    int status = 0;

    if (pipe(fildes) == -1) {
        perror("pipe()");

        return 1;
    }

    proc = fork();
    switch (proc) {
        case -1:
            /* error code */
            perror("fork()");

            return 1;    
        case 0:
            /* child code */
            close(fildes[0]);

            memset(buffer, 'x', sizeof(buffer));

            int count = 0;
            while ((len = write(fildes[1], buffer, sizeof(buffer))) > 0) {
                printf("%d | sending '%s' to process %d (%d)\n", getpid(), buffer, getppid(), count);
                count++;
            }

            close(fildes[1]);
            if (len == -1) {
                perror("write()");

                _exit(1);
            }

            _exit(0);
            break;
        default:
            /* parent code */
            close(fildes[1]);

            pid_t brother = fork();

            if (brother == -1) {
                perror("fork()");

                return 1;
            } else if (brother == 0) {
                /* child process */
                execlp("sleep", "sleep", "5", NULL);

                perror("sleep");
                _exit(127);
            }

            if (wait(&status) == -1) {
                perror("wait()");
                return 1;
            }

            if (WIFEXITED(status) != 0 && WEXITSTATUS(status) != 0) {
                printf("error on exec()");

                return 2;
            }

            while ((len = read(fildes[0], buffer, sizeof(buffer))) > 0) {
                printf("%d | received '%s' from process %d\n", getpid(), buffer, proc);
            }

            if (len == -1) {
                perror("read()");
                return 1;
            }

            break;
    }

    return 0;
}