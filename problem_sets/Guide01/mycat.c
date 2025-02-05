#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv) {
    char buffer[BUFSIZ];
    ssize_t bytes_read = 0;
    int file_descriptor = STDIN_FILENO;

    if (argc == 2) {
        // open the source file
        file_descriptor = open(argv[1], O_RDONLY);
        if (file_descriptor == -1) {
            perror(argv[1]);
            return 1;
        }
    }

    // read buffer size bytes from the source file (or stdin)
    while ((bytes_read = read(file_descriptor, buffer, sizeof(buffer))) > 0)
        write(STDOUT_FILENO, buffer, bytes_read);

    // close the file
    if (file_descriptor > STDERR_FILENO)
        close(file_descriptor);

    return 0;
}