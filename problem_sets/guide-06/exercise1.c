#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(void) {

    // open /etc/passwd
    int input = open("/etc/passwd", O_RDONLY, 0600);
    if (input == -1) {
        perror("open()");
        return 1;
    }

    // redirect stdin to /etc/passwd
    if (dup2(input, STDIN_FILENO) == -1) {
        perror("dup2()");
        return 1;
    }
    close(input);

    // store stdout
    int original_out = dup(STDOUT_FILENO);
    if (original_out == -1) {
        perror("dup()");
        return 1;
    }

    // open saida.txt
    int output = open("saida.txt", O_CREAT | O_TRUNC | O_WRONLY, 0600);
    if (output == -1) {
        perror("open()");
        return 1;
    }

    // redirect stdout to saida.txt
    if (dup2(output, STDOUT_FILENO) == -1) {
        perror("dup2()");
        return 1;
    }
    close(output);

    // open erros.txt
    int error = open("erros.txt", O_CREAT | O_TRUNC | O_WRONLY, 0600);
    if (error == -1) {
        perror("open()");
        return 1;
    }

    // redirect stderr to erros.txt
    if (dup2(error, STDERR_FILENO) == -1) {
        perror("dup2()");
        return 1;
    }
    close(error);

    // restore stdout
    if (dup2(original_out, STDOUT_FILENO) == -1) {
        perror("dup2()");
        return 1;
    }

    if (write(STDOUT_FILENO, "finished\n", sizeof("finished\n")) == -1) {
        perror("write()");
        return 1;
    }

    return 0;
}