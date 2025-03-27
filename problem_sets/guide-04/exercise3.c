#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VALUE 100

#define ROWS 10
#define COLUMNS 10

struct position {
    int row;
    int column;
};

int **create_matrix() {
    int **matrix = (int **) malloc(sizeof(int *) * ROWS);
    if (matrix == NULL) {
        perror("malloc()");
        return NULL;
    }

    int *rows = (int *) malloc(sizeof(int) * ROWS * COLUMNS);

    if (matrix == NULL) {
        perror("malloc()");
        return NULL;
    }

    int i;
    for (i = 0; i < ROWS; i++) {
        matrix[i] = rows + i * ROWS;
    }

    // memset(rows, 0, sizeof(int) * ROWS * COLUMNS);

    for (i = 0; i < ROWS * COLUMNS; i++) {
        rows[i] = rand() % MAX_VALUE;
    }

    return matrix;
}

void destroy_matrix(int **matrix) {
    if (matrix != NULL) {
        if (matrix[0] != NULL) {
            free(matrix[0]);
        }

        free(matrix);
    }
}

void show_matrix(int **matrix) {
    int i, j;

    printf("   | ");
    for (i = 0; i < COLUMNS; i++)
        printf("%5d ", i);

    printf("\n");
    for (i = 0; i < COLUMNS * 6 + 4; i++)
        printf("-");

    printf("\n");

    for (i = 0; i < ROWS; i++) {
        printf("%2d | ", i);

        for (j = 0; j < COLUMNS; j++)
            printf("%5d ", matrix[i][j]);

        printf("\n");
    }
}

int lookup_value(int **matrix, int value, struct position *storage) {
    struct position found;
    pid_t child = 0;
    ssize_t len = 0;
    int fildes[2];
    int i, j;

    /* pipe error */
    if (pipe(fildes) == -1) {
        perror("pipe()");

        return -1;
    }

    for (i = 0; i < ROWS; i++) {
        child = fork();

        switch (child) {
            case -1:
                /* error code */
                perror("fork()");

                return -1;
            case 0:
                /* child code */
                close(fildes[0]);

                for (j = 0; j < COLUMNS; j++) {
                    if (matrix[i][j] == value) {
                        found.row = i;
                        found.column = j;

                        /* send the position to the parent */
                        len = write(fildes[1], &found, sizeof(found));

                        if (len == -1) {
                            perror("write()");

                            _exit(1);
                        }
                    }
                }

                close(fildes[1]);

                _exit(0);
            default:
                /* parent code */

                break;
        }
    }

    /* must close it after, because of other processes */
    close(fildes[1]);

    int count = 0;
    while ((len = read(fildes[0], &found, sizeof(found))) > 0) {
        storage[count].row = found.row;
        storage[count].column = found.column;

        count++;
    }

    close(fildes[0]);

    if (len == -1) {
        perror("read()");

        return -1;
    }

    return count;
}

int main(void) {
    srand(time(0));

    int **m = create_matrix();
    // m[0][5] = 45;
    // m[3][6] = 45;
    // m[9][8] = 45;
    // m[1][1] = 45;
    // m[7][2] = 45;

    show_matrix(m);

    int value = 45, len = 0, i, j;
    struct position storage[ROWS];

    // printf("searching for %d\n", value);
    // len = lookup_value(m, value, storage);

    for (i = 0; i < 5; i++) {
        value = rand() % MAX_VALUE;
        printf("searching for %d\n", value);
        len = lookup_value(m, value, storage);

        for (j = 0; j < len; j++) {
           printf("(%d, %d) ", storage[j].row, storage[j].column);
        }
        if (j == 0)
            printf("value was not found");
        printf("\n");
        
    }

    // for (i = 0; i < len; i++) {
        // printf("(%d, %d) ", storage[i].row, storage[i].column);
    // }

    destroy_matrix(m);

    return 0;
}


