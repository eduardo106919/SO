#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_NAME "fifo"

int main(void) {
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo()");
        return 1;
    }

    printf("Ligação '%s' iniciada\n", FIFO_NAME);
    int fifo = open(FIFO_NAME, O_RDONLY);
    if (fifo == -1) {
        perror("open()");
        return 1;
    }
    printf("Ligação '%s' estabelecida\n", FIFO_NAME);

    close(fifo);

    return 0;
}