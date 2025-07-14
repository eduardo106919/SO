#include "matrix.h"
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#define MAX_VALUE 100
#define ROWS 10
#define COLUMNS 10

int **create_matrix() {
    // seed random numbers
    srand(time(NULL));

    // allocate matrix
    int **matrix = (int **) malloc(sizeof(int *) * ROWS);
    if (matrix == NULL)
        return NULL;

    for (size_t i = 0; i < ROWS; i++) {
        matrix[i] = (int *) malloc(sizeof(int) * COLUMNS);
        if (matrix[i] == NULL)
            return NULL;

        // populate each line with random numbers (or zeros)
        for (size_t j = 0; j < COLUMNS; j++) {
            matrix[i][j] = rand() % MAX_VALUE;
            // matrix[i][j] = 0;
        }
    }

    /*
    matrix[0][5] = 10;
    matrix[3][0] = 10;
    matrix[5][6] = 34;
    matrix[9][1] = 67;
    */

    return matrix;
}

void destroy_matrix(int **matrix) {
    if (matrix != NULL) {
        for (int i = 0; i < ROWS; i++)
            if (matrix[i] != NULL)
                free(matrix[i]);

        free(matrix);
    }
}

void print_matrix(int **matrix) {
    printf("   | ");
    for (int i = 0; i < COLUMNS; i++)
        printf("%5d ", i);

    printf("\n");
    for (int i = 0; i < COLUMNS * 6 + 4; i++)
        printf("-");

    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        printf("%2d | ", i);

        for (int j = 0; j < COLUMNS; j++)
            printf("%5d ", matrix[i][j]);

        printf("\n");
    }
}

bool value_exists(int **matrix, int value) {
    size_t i = 0, j = 0;
    pid_t identifier;
    for (i = 0; i < ROWS; i++) {
        identifier = fork();

        // child process
        if (identifier == 0) {
            // iterate over the line
            for (j = 0; j < COLUMNS; j++)
                if (matrix[i][j] == value)
                    _exit(i);

            _exit(ROWS);
        } else if (identifier == -1) {
            perror("fork()");
            return false;
        }
    }

    int status = 0;
    bool out = false;
    for (i = 0; i < ROWS; i++) {
        // waits for each child to find the value
        identifier = wait(&status);
        if (identifier == -1) {
            perror("wait()");
            return false;
        }

        if (WIFEXITED(status) != 0) {
            if (out == false && WEXITSTATUS(status) < ROWS)
                out = true;
        }
    }

    return out;
}

void lines_with_value(int **matrix, int value) {
    size_t i = 0, j = 0;
    pid_t identifier;
    for (i = 0; i < ROWS; i++) {
        identifier = fork();

        // child process
        if (identifier == 0) {
            // iterate over the line
            for (j = 0; j < COLUMNS; j++)
                if (matrix[i][j] == value) {
                    _exit(i);
                }

            _exit(ROWS);
        } else if (identifier == -1) {
            perror("fork");
            return;
        }
    }

    /*
        if any process ends before the parent gets to the second loop
        they stay in zombie mode, keeping the exit status and it's identifier,
        so when the parent gets to the first wait(), gets the status of the zombie process first
    */

    int status = 0, out;
    for (i = 0; i < ROWS; i++) {
        // waits for each child to find the value
        identifier = wait(&status);

        if (identifier != -1 && WIFEXITED(status) != 0) {
            out = WEXITSTATUS(status);
            if (out < ROWS && out >= 0)
                printf("%d found at row %u\n", value, out);
        }
    }
}
