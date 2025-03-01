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
    printf("-> searching for %d\n", value);

    int row = value_exists(matrix, value);
    if (row >= 0) {
        printf("%d found at row %d\n", value, row);
    } else if (row == -1) {
        printf("%d was not found\n", value);
    } else {
        // fork() or wait() error
        return 1;
    }

    // free the matrix
    destroy_matrix(matrix);

    return 0;
}