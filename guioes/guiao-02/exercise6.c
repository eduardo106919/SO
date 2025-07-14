#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VALUE 100

int main(void) {
    srand(time(0));

    // generate random matrix
    int **matrix = create_matrix();
    int value = 0;

    // print matrix
    print_matrix(matrix);


    value = rand() % MAX_VALUE;
    printf("searching for %d\n", value);
    lines_with_value(matrix, value);

    value = rand() % MAX_VALUE;
    printf("searching for %d\n", value);
    lines_with_value(matrix, value);

    value = rand() % MAX_VALUE;
    printf("searching for %d\n", value);
    lines_with_value(matrix, value);


    // free the matrix
    destroy_matrix(matrix);

    return 0;
}