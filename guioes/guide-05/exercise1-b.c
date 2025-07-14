#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_NAME "fifo"

int main(void) {
    int fifo = open(FIFO_NAME, O_WRONLY);
    if (fifo == -1) {
        perror("open()");
        return 1;
    }

    char buffer[BUFSIZ];
    ssize_t len = 0;
    ssize_t out = 0;
    while ((len = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
        out = write(fifo, buffer, len);
        if (out == -1) {
            perror("write()");
            close(fifo);
            return 1;
        }
    }
    close(fifo);

    if (len == -1) {
        perror("read()");
        return 1;
    }

    return 0;
}