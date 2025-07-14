#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    srandom(time(NULL));

    int res = random() % 3;

    return res;
}
