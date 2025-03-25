#include "defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
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

    // create response channel
    if (mkfifo(CLIENT, 0666) == -1) {
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
    Msg request;

    request.occurrences = 0;
    // shut down server
    if (strcmp(argv[1], "-f") == 0) {
        request.pid = -1;
        request.needle = 0;
    } else {
        request.pid = 0;
        request.needle = atoi(argv[1]);
    }

    // send request
    out = write(output, &request, sizeof(request));
    close(output);

    if (out == -1) {
        perror("write()");
        return 1;
    }

    if (strcmp(argv[1], "-f") != 0) {
        // open response channel
        int input = open(CLIENT, O_RDONLY);
        if (input == -1) {
            perror("open()");
            return 1;
        }

        // receive answer
        out = read(input, &request, sizeof(request));
        close(input);
        unlink(CLIENT);

        if (out == -1) {
            perror("read()");
            return 1;
        }

        printf("count %d: %d\n", request.needle, request.occurrences);
    } else {
        unlink(CLIENT);
    }

    return 0;
}