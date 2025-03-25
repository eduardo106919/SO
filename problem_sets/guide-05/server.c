#include "defs.h"
#include "vector.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void) {
    printf("server is online\n");

    init_vector();
    print_vector();

    int input = 0, output = 0;
    ssize_t out = 0;
    char needle[20];
    int count = 0;
    int stop = 1;

    // create request channel
    if (mkfifo(SERVER, 0666) == -1) {
        perror("mkfifo()");
        return 1;
    }

    while (stop != 0) {
        // open request channel
        input = open(SERVER, O_RDONLY);
        if (input == -1) {
            perror("open() 1");
            return 1;
        }

        char client_fifo[50];
        // receive client name
        out = read(input, client_fifo, sizeof(client_fifo));
        // receive request
        out = read(input, needle, sizeof(needle));
        close(input);

        if (out == -1) {
            perror("read()");
            unlink(SERVER);

            return 1;
        }

        // shut down server
        if (strcmp(needle, "-f") == 0) {
            printf("server is shutting down\n");
            close(input);
            unlink(SERVER);

            stop = 0;
        } else {
            count = count_needle(atoi(needle));

            // open response channel
            output = open(client_fifo, O_WRONLY);
            if (output == -1) {
                perror("open() 2");
                return 1;
            }

            // send response
            out = write(output, &count, sizeof(count));
            close(output);

            if (out == -1) {
                perror("write()");
                return 1;
            }
        }
    }

    return 0;
}
