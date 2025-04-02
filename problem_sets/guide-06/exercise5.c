#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>


int main(void) {

    int fildes[2];

    if (pipe(fildes) == -1) {
        perror("pipe()");
        return 1;
    }

    pid_t proc = fork();

    switch (proc) {
    case -1:
        /* error code */
        perror("fork()");
        return 1;
    case 0:
        /* child code */
        close(fildes[0]); // close reading side

        // redirect stdout to the writting side of the pipe
        if (dup2(fildes[1], STDOUT_FILENO) == -1) {
            perror("dup2()");
            _exit(1);
        }

        execlp("ls", "ls", "/etc", NULL);

        perror("ls -l");
        _exit(127);
    
    default:
        /* parent code */
        close(fildes[1]); // close writting side

        printf("ls /etc | wc -l\n");

        proc = fork();
        if (proc == -1) {
            perror("fork()");
            return 1;
        } else if (proc == 0) {
            
            // redirect stdin to the reading side of the pipe
            if (dup2(fildes[0], STDIN_FILENO) == -1) {
                perror("dup2()");
                _exit(1);
            }

            execlp("wc", "wc", "-l", NULL);
            perror("wc -l");
            _exit(127);
        }
        break;
    }

    return 0;
}