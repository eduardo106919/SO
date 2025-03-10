#include <unistd.h>
#include <stdio.h>

int main(void) {

    execlp("ls", "ls", "-l", NULL);
    printf("Something went wrong with execlp()!!\n");

    return 0;
}