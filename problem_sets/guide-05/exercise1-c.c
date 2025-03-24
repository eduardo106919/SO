#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_NAME "fifo"

int main(void) {
    int fifo = open(FIFO_NAME, O_RDONLY);
    if (fifo == -1) {
        perror("open()");
        return 1;
    }

    char buffer[BUFSIZ];
    ssize_t len = 0;
    ssize_t out = 0;
    while ((len = read(fifo, buffer, sizeof(buffer))) > 0) {
        out = write(STDOUT_FILENO, buffer, len);
        if (out == -1) {
            perror("write()");
            close(fifo);
            return 1;
        }
    }
    close(fifo);
    unlink(FIFO_NAME);

    if (len == -1) {
        perror("read()");
        return 1;
    }

    return 0;
}