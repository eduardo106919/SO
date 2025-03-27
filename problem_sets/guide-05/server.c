#include "defs.h"
#include "vector.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUF_SIZE 50

int main(void) {
    printf("server is online\n");

    init_vector();
    print_vector();

    int input = 0, output = 0;
    ssize_t out = 0;
    Msg message;
    int stop = 1;
    char client_fifo[BUF_SIZE];

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
        
        // receive request
        out = read(input, &message, sizeof(message));
        close(input);

        if (out == -1) {
            perror("read()");
            unlink(SERVER);

            return 1;
        }

        // shut down server
        if (out > 0 && message.pid == -1) {
            printf("server is shutting down\n");
            unlink(SERVER);

            stop = 0;
        } else {
            message.occurrences = count_needle(message.needle);

            sprintf(client_fifo, "%s_%d", CLIENT, message.pid);
            // open response channel
            output = open(client_fifo, O_WRONLY);
            if (output == -1) {
                perror("open() 2");
                return 1;
            }

            // send response
            out = write(output, &message, sizeof(message));
            close(output);

            if (out == -1) {
                perror("write()");
                return 1;
            }
        }
    }

    return 0;
}
