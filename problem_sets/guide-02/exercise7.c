#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VALUE 100

int main(void) {
    srand(time(0));

    // create a random matrix
    int **matrix = create_matrix();

    // print the matrix
    print_matrix(matrix);

    char *file_name = "matrix.txt";
    if (write_matrix_file(matrix, file_name) != 0) {
        return 1;
    }

    int value = rand() % MAX_VALUE;
    // int value = 10;
    printf("-> searching for %d\n", value);

    // find_value_file(file_name, value);

    int row = find_value_file(file_name, value);
    if (row >= 0) {
        printf("%d found at row %d\n", value, row);
    } else if (row == -1) {
        printf("%d was not found\n", value);
    } else {
        // fork() or wait() error
        return 1;
    }

    destroy_matrix(matrix);

    return 0;
}