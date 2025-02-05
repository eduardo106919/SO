#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>

/**
 * ----------- Time Analysis -----------
 * file aux.txt has 100 MB
 * 
 * buffer size == 10 bytes
 * time == 15.09 seconds
 * 
 * buffer size == 100 bytes
 * time == 1.62 seconds
 * 
 * buffer size == 1000 bytes
 * time == 0.265 seconds
 * 
 * buffer size == BUFSIZ (8192 bytes)
 * time == 0.121 seconds
 * 
 * ------------------------------------
 * 
 * command to fill a file with zeros:
 *      dd if=/dev/zero of=filename bs=size count=1
 * 
 * example:
 *      dd if=/dev/zero of=aux.txt bs=100M count=1
 * 
 */

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stdout, "Usage: '%s' [src-file] [dest-file]\n", argv[0]);
        return 1;
    }

    // open the source file
    int input_file = open(argv[1], O_RDONLY);
    if (input_file == -1) {
        perror(argv[1]);
        return 2;
    }

    // open the destiny file
    int output_file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (output_file == -1) {
        perror(argv[2]);
        return 2;
    }

    size_t bytes_read = 0;
    char buffer[BUFSIZ]; // change the buffer size here

    // read BUFSIZ bytes from the source file
    while ((bytes_read = read(input_file, buffer, sizeof(buffer))) > 0)
        write(output_file, buffer, bytes_read);

    close(input_file);
    close(output_file);

    return 0;
}