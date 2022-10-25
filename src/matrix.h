#ifndef SRC_MATRIX_H_
#define SRC_MATRIX_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define N 250

typedef struct Matrix {
  double** matrix;
  int rows;
  int columns;
} matrix_t;

matrix_t create_matrix(int rows, int columns);
void remove_matrix(matrix_t* A);
matrix_t mult_number(matrix_t* A, double number);
void set_rot_matrix_x(matrix_t* mat, double angle);
void set_rot_matrix_y(matrix_t* mat, double angle);
void set_rot_matrix_z(matrix_t* mat, double angle);
matrix_t mult_matrix(matrix_t* A, matrix_t* B);
int eq_matrix(matrix_t* A, matrix_t* B);
void print_matrix(matrix_t neo);

#endif  // SRC_MATRIX_H_
