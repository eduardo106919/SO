#include "defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUF_SIZE 50

void usage(const char *command) {
    printf("Usage:\n");
    printf("%s <value>\n", command);
    printf("%s -f\n", command);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    char client_fifo[BUF_SIZE];
    sprintf(client_fifo, "%s_%d", CLIENT, getpid());

    Msg message;
    message.occurrences = 0;
    message.needle = 0;

    // determine whether to shut down or count value
    if (strcmp(argv[1], "-f") == 0) {
        message.pid = -1;
    } else {
        message.needle = atoi(argv[1]);
        message.pid = getpid();
    }

    // create response channel
    if (mkfifo(client_fifo, 0666) == -1) {
        perror("mkfifo()");
        return 1;
    }

    // open request channel
    int output = open(SERVER, O_WRONLY);
    if (output == -1) {
        perror("open()");
        return 1;
    }

    ssize_t out = 0;
    // send request
    out = write(output, &message, sizeof(message));
    close(output);

    if (out == -1) {
        perror("write()");
        return 1;
    }

    if (strcmp(argv[1], "-f") != 0) {
        // open response channel
        int input = open(client_fifo, O_RDONLY);
        if (input == -1) {
            perror("open()");
            return 1;
        }

        // receive answer
        out = read(input, &message, sizeof(message));
        close(input);
        unlink(client_fifo);

        if (out == -1) {
            perror("read()");
            return 1;
        }

        printf("count %d: %d\n", message.needle, message.occurrences);
    } else {
        unlink(client_fifo);
    }

    return 0;
}