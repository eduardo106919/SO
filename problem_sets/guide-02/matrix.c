#include "matrix.h"
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>

#define MAX_RAND 100
#define ROWS 10
#define COLUMNS 10

int **create_matrix() {
    // seed random numbers
    srand(time(NULL));

    // allocate matrix
    int **matrix = (int **) malloc(sizeof(int *) * ROWS);
    if (matrix == NULL)
        return NULL;

    // populate with random numbers
    for (int i = 0; i < ROWS; i++) {
        matrix[i] = (int *) malloc(sizeof(int) * COLUMNS);
        if (matrix[i] == NULL)
            return NULL;

        for (int j = 0; j < COLUMNS; j++)
            matrix[i][j] = rand() % MAX_RAND;
    }

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

int value_exists(int **matrix, int value) {
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
            return -2;
        }
    }

    int status = 0, out = -1;
    for (i = 0; i < ROWS; i++) {
        // waits for each child to find the value
        identifier = wait(&status);
        if (identifier == -1) {
            perror("wait()");
            return -2;
        }

        if (WIFEXITED(status) != 0) {
            if (WEXITSTATUS(status) < ROWS)
                out = WEXITSTATUS(status);
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

int write_matrix_file(int **matrix, const char *file_name) {
    // open the file and clean it
    int file = open(file_name, O_CREAT | O_TRUNC | O_RDWR, 0600);
    if (file == -1) {
        perror(file_name);
        return 1;
    }

    size_t bytes_written = 0;
    // write every line in the file
    for (size_t i = 0; i < ROWS; i++) {
        bytes_written = write(file, matrix[i], sizeof(matrix[i]));
        if (bytes_written == -1) {
            perror("write()");
            return 1;
        }
    }

    close(file);

    return 0;
}

/**
 * @brief Searchs value in a matrix that is written in a file
 * 
 * @param file_name name of the file to read from
 * @param value value to search for
 * @return -2 on error, -1 value was not found, > 0 on sucess
 */
int find_value_file(const char *file_name, int value) {
    // open the file for reading
    int file = open(file_name, O_RDONLY);
    if (file == -1) {
        perror(file_name);
        return -2;
    }

    pid_t identifier;
    size_t i = 0, j = 0, bytes_read = 0;
    int buffer[ROWS][COLUMNS];

    for (i = 0; i < ROWS; i++) {
        // read a line from the file
        bytes_read = read(file, buffer[i], sizeof(buffer[i]));
        if (bytes_read == -1) {
            perror("read()");
            close(file);

            return -2;
        }

        identifier = fork();

        // child process
        if (identifier == 0) {
            for (j = 0; j < COLUMNS; j++) {
                if (buffer[i][j] == value) {
                    _exit(i);
                }
            }

            _exit(ROWS);
        } else if (identifier == -1) {
            // fork() error
            perror("fork()");
            close(file);

            return -2;
        }
    }

    int status = 0, out, row = -1;

    for (i = 0; i < ROWS; i++) {
        // wait for every child, and get it's exit code
        identifier = wait(&status);

        if (identifier != -1 && WIFEXITED(status) != 0) {
            out = WEXITSTATUS(status);
            if (out < ROWS && out >= 0) {
                row = out;
            }
        } else if (identifier == -1) {
            // wait() error
            perror("wait()");
            close(file);

            return -2;
        }
    }

    close(file);

    return row;
}