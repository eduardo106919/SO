#include <unistd.h>
#include <stdio.h>

int main(void) {

    execlp("ls", "ls", "-l", NULL);
    perror("ls");

    return 1;
}