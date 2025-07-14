#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>

#define MAX_VALUE 100

/* Creates a matrix of random integers, of size ROWS x COLUMNS */
int **create_matrix();

/* Frees the allocated space for a matrix */
void destroy_matrix(int **matrix);

/* Shows a matrix */
void print_matrix(int **matrix);

/* Writes a matrix in a file */
int write_matrix_in_file(int **matrix, const char *file_name);

/* checks if value exists in the given file */
bool value_exists(const char *file_name, int value);

/* Searchs for value in the given file */
void lines_with_value(const char *file_name, int value);

#endif