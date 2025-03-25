#include "defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

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

    char client_fifo[50];
    sprintf(client_fifo, "%s_%d", CLIENT, getpid());

    // open request channel
    int output = open(SERVER, O_WRONLY);
    if (output == -1) {
        perror("open()");
        return 1;
    }

    // send client name
    ssize_t out = write(output, client_fifo, strlen(client_fifo) + 1);
    // send request
    out = write(output, argv[1], strlen(argv[1]) + 1);
    close(output);

    if (out == -1) {
        perror("write()");
        return 1;
    }

    if (strcmp(argv[1], "-f") != 0) {

        // create response channel
        if (mkfifo(client_fifo, 0666) == -1) {
            perror("mkfifo()");
            return 1;
        }

        // open response channel
        int input = open(client_fifo, O_RDONLY);
        if (input == -1) {
            perror("open()");
            return 1;
        }

        int count = 0;
        // receive answer
        out = read(input, &count, sizeof(count));
        close(input);
        unlink(client_fifo);

        if (out == -1) {
            perror("read()");
            return 1;
        }

        printf("count %s: %d\n", argv[1], count);
    }

    return 0;
}