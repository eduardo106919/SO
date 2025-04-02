#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(void) {

    pid_t proc = 0;
    int pipefd[2];
    if (pipe(pipefd) == -1) {
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
        close(pipefd[1]); // close writting side of the pipe

        // redirect stdin to the reading side of the pipe
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2()");
            _exit(1);
        }

        // execute word count
        execlp("wc", "wc", NULL);

        /* error on execlp() */
        perror("wc");
        _exit(127);
    
    default:
        /* parent code */
        close(pipefd[0]); // close reading side of the pipe
        
        ssize_t out = 0;
        char buffer[BUFSIZ];

        // read data from the user
        while ((out = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
            // send data to the child process
            out = write(pipefd[1], buffer, out);
            if (out == -1) {
                perror("write()");
                return 1;
            }
        }

        break;
    }

    return 0;
}