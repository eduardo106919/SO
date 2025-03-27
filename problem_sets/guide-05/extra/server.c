#include "defs.h"
#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LOG_NAME "counts.log"
#define BUF_SIZE 50

pid_t turn_register_on(int fildes[2]) {
    pid_t proc = fork();

    switch (proc) {
        case -1:
            /* error code */
            perror("fork()");
        case 0:
            /* child code */
            close(fildes[1]); // close writing side

            Msg response;
            char buffer[BUF_SIZE];
            ssize_t out = 0;

            // open log file
            int log_file = open(LOG_NAME, O_CREAT | O_WRONLY | O_APPEND, 0600);
            if (log_file == -1) {
                perror("open()");
                _exit(1);
            }

            printf("[server is writting in %s]\n", LOG_NAME);

            // receive ocurrences from processes
            while ((out = read(fildes[0], &response, sizeof(response))) > 0) {
                sprintf(buffer, "[%d] searched %d - count %d\n", response.pid, response.needle, response.occurrences);

                // write in the log file
                out = write(log_file, buffer, strlen(buffer) + 1);
                if (out == -1) {
                    perror("write() 1");
                    close(log_file);
                    _exit(1);
                }
            }

            close(fildes[0]);
            close(log_file);

            if (out == -1) {
                perror("read()");
                _exit(1);
            }

            printf("[finished writting in %s]\n", LOG_NAME);

            _exit(0);
        default:
            /* parent code */
            close(fildes[0]); // close reading side

            break;
    }

    return proc;
}


int shut_down_server(int fildes[2], int n_procs) {
    pid_t proc = 0;
    int status = 0;

    while (n_procs > 0) {
        proc = wait(&status);

        if (proc == -1) {
            perror("wait()");
            return 1;
        }

        if (WIFEXITED(status) != 0) {
            if (WEXITSTATUS(status) != 0) {
                printf("[%d] returned %d\n", proc, WEXITSTATUS(status));
            }
        }

        n_procs--;
    }

    close(fildes[1]); // close writting side of register

    // wait for register
    proc = wait(&status);

    if (proc == -1) {
        perror("wait()");
        return 1;
    }

    if (WIFEXITED(status) != 0) {
        if (WEXITSTATUS(status) != 0) {
            printf("register [%d] returned %d\n", proc, WEXITSTATUS(status));
        }
    }

    return 0;
}


int main(int argc, char **argv) {
    printf("[server is online]\n");

    pid_t proc = 0;
    ssize_t out = 0;
    Msg response;
    int n_procs = 0;
    char client_fifo[BUF_SIZE];

    int fildes[2];
    if (pipe(fildes) == -1) {
        perror("pipe()");
        return 1;
    }

    // turn the log file register on
    proc = turn_register_on(fildes);
    if (proc <= 0) {
        printf("register did not start\n");

        close(fildes[1]);

        return 1;
    }

    init_vector();
    print_vector();

    // create request channel
    if (mkfifo(SERVER, 0666) == -1) {
        perror("mkfifo()");
        return 1;
    }

    int stop = 1;
    int input = 0, output = 0;

    while (stop != 0) {

        // open request channel
        input = open(SERVER, O_RDONLY);
        if (input == -1) {
            perror("open()");
            return 1;
        }

        // receive request
        out = read(input, &response, sizeof(response));
        close(input);

        if (out == -1) {
            perror("read()");
            return 1;
        }

        // shut down server
        if (out > 0 && response.pid == -1) {
            out = shut_down_server(fildes, n_procs);

            stop = 0;
        } else if (out > 0) {

            // create process to look for the needle
            proc = fork();

            switch (proc) {
                case -1:
                    /* error code */
                    perror("fork()");

                    unlink(SERVER);

                    return 1;

                case 0:
                    /* child process */

                    // reading side of the pipe with the register is already closed

                    sprintf(client_fifo, "%s_%d", CLIENT, response.pid);

                    response.occurrences = count_needle(response.needle);

                    // open response channel
                    output = open(client_fifo, O_WRONLY);
                    if (output == -1) {
                        perror("open()");

                        _exit(1);
                    }

                    // send response to client
                    out = write(output, &response, sizeof(response));
                    close(output);
                    if (out == -1) {
                        perror("write()");

                        _exit(1);
                    }

                    // send counting to register
                    out = write(fildes[1], &response, sizeof(response));
                    close(fildes[1]); // close writting side of the anon pipe

                    if (out == -1) {
                        perror("write()");

                        _exit(1);
                    }

                    _exit(0);
                default:
                    /* parent process */

                    n_procs++;

                    break;
            }
        }
    }

    unlink(SERVER);

    return 0;
}