#include <unistd.h>
#include <stdio.h>

int main(void) {

    printf("Process id: %d\n", getpid());
    printf("Parent id:  %d\n", getppid());

    return 0;
}