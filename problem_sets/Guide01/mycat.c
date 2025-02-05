#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv) {
    char buffer[BUFSIZ];
    ssize_t bytes_read = 0;
    int file_descriptor = STDIN_FILENO;

    if (argc == 2) {
        file_descriptor = open(argv[1], O_RDONLY);
        if (file_descriptor == -1) {
            perror("Error");
            return 1;
        }
    }

    while ((bytes_read = read(file_descriptor, buffer, BUFSIZ)) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    return 0;
}