
#include "matrix.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


int main(void) {

    char * filename = "matrix.bin";
    // generate random matrix
    int **matrix = create_matrix();
    int value = 0;

    // print matrix
    print_matrix(matrix);

    write_matrix_in_file(matrix, filename);

    destroy_matrix(matrix);

    value = rand() % MAX_VALUE;
    printf("searching for %d\n", value);

    bool row = value_exists(filename, value);
    if (row == true)
        printf("value %d found in the matrix\n", value);
    else
        printf("value %d not found in the matrix\n", value);

    value = rand() % MAX_VALUE;
    printf("searching for %d\n", value);

    row = value_exists(filename, value);
    if (row == true)
        printf("value %d found in the matrix\n", value);
    else
        printf("value %d not found in the matrix\n", value);

    value = rand() % MAX_VALUE;
    printf("searching for %d\n", value);

    row = value_exists(filename, value);
    if (row == true)
        printf("value %d found in the matrix\n", value);
    else
        printf("value %d not found in the matrix\n", value);


    value = rand() % MAX_VALUE;
    printf("searching for %d\n", value);
    lines_with_value(filename, value);

    value = rand() % MAX_VALUE;
    printf("searching for %d\n", value);
    lines_with_value(filename, value);

    value = rand() % MAX_VALUE;
    printf("searching for %d\n", value);
    lines_with_value(filename, value);


    return 0;
}