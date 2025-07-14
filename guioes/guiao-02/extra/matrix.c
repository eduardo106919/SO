
#include "matrix.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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

int write_matrix_in_file(int **matrix, const char *file_name) {
    // open the file and clean it
    int file = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0600);
    if (file == -1) {
        perror(file_name);
        return 1;
    }

    size_t bytes_written = 0;
    for (size_t i = 0; i < ROWS; i++) {
        bytes_written = write(file, matrix[i], sizeof(int) * COLUMNS);
        if (bytes_written == -1) {
            close(file);
            perror("write()");
            return 2;
        }
    }

    close(file);

    return 0;
}

bool value_exists(const char *file_name, int value) {
    int file = 0;
    size_t offset = COLUMNS * sizeof(int);
    pid_t id = 0;
    int temp = 0;

    size_t i = 0, j = 0;
    for (i = 0; i < ROWS; i++) {
        id = fork();

        if (id == 0) {
            file = open(file_name, O_RDONLY);
            if (file == -1) {
                perror("open()");
                _exit(ROWS);
            }

            // go to the correct row
            if (lseek(file, offset * i, SEEK_SET) == -1) {
                close(file);
                perror("lseek()");
                _exit(ROWS);
            }

            // iterate over the row
            for (j = 0; j < COLUMNS; j++) {
                if (read(file, &temp, sizeof(temp)) == -1) {
                    close(file);
                    perror("read()");
                    _exit(ROWS);
                }

                // found the value
                if (temp == value) {
                    close(file);
                    _exit(i);
                }
            }

            close(file);
            _exit(ROWS);
        } else if (id == -1) {
            perror("fork()");
            return false;
        }
    }

    int status = 0;
    bool out = false;
    for (i = 0; i < ROWS; i++) {
        id = wait(&status);
        if (id == -1) {
            perror("wait()");
            return out;
        }

        if (WIFEXITED(status) != 0) {
            if (out == false && WEXITSTATUS(status) < ROWS) {
                out = true;
            }
        }
    }

    return out;
}

void lines_with_value(const char *file_name, int value) {
    int file = 0;
    size_t offset = COLUMNS * sizeof(int);
    pid_t id = 0;
    int temp = 0;

    size_t i = 0, j = 0;
    for (i = 0; i < ROWS; i++) {
        id = fork();

        if (id == 0) {
            file = open(file_name, O_RDONLY);
            if (file == -1) {
                perror("open()");
                _exit(ROWS);
            }

            // go to the correct row
            if (lseek(file, offset * i, SEEK_SET) == -1) {
                close(file);
                perror("lseek()");
                _exit(ROWS);
            }

            // iterate over the row
            for (j = 0; j < COLUMNS; j++) {
                if (read(file, &temp, sizeof(temp)) == -1) {
                    close(file);
                    perror("read()");
                    _exit(ROWS);
                }

                // found the value
                if (temp == value) {
                    close(file);
                    _exit(i);
                }
            }

            close(file);
            _exit(ROWS);
        } else if (id == -1) {
            perror("fork()");
            return;
        }
    }

    int status = 0;
    for (i = 0; i < ROWS; i++) {
        id = wait(&status);
        if (id == -1) {
            perror("wait()");
            return;
        }

        if (WIFEXITED(status) != 0) {
            if (WEXITSTATUS(status) < ROWS) {
                printf("value %d found in row %d\n", value, WEXITSTATUS(status));
            }
        }
    }
}
