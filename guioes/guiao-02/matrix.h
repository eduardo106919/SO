#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>

/* Creates a matrix of random integers, of size ROWS x COLUMNS */
int **create_matrix();

/* Frees the allocated space for a matrix */
void destroy_matrix(int **matrix);

/* Shows a matrix */
void print_matrix(int **matrix);

/* Checks if values exists in the matrix */
bool value_exists(int **matrix, int value);

/* Shows the lines in which value occurs */
void lines_with_value(int **matrix, int value);

/* Writes a matrix in a file */
int write_matrix_file(int **matrix, const char *file_name);

/* Searchs for value in the selected file */
int find_value_file(const char *file_name, int value);

#endif