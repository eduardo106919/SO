#include <unistd.h>
#include <stdio.h>

int main(void) {

    printf("Current id: %d\n", getpid());
    printf("Parrent id: %d\n", getppid());

    return 0;
}