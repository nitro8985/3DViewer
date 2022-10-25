#include "matrix.h"

// Создание матрицы вращения по x
void set_rot_matrix_x(matrix_t *mat, double angle) {
  mat->matrix[0][0] = 1;
  mat->matrix[1][1] = cos(angle);
  mat->matrix[1][2] = -1 * sin(angle);
  mat->matrix[2][1] = sin(angle);
  mat->matrix[2][2] = cos(angle);
}

// Создание матрицы вращения по y
void set_rot_matrix_y(matrix_t *mat, double angle) {
  mat->matrix[0][0] = cos(-angle);
  mat->matrix[0][2] = -1 * sin(-angle);
  mat->matrix[1][1] = 1;
  mat->matrix[2][0] = sin(-angle);
  mat->matrix[2][2] = cos(-angle);
}

// Создание матрицы вращения по z
void set_rot_matrix_z(matrix_t *mat, double angle) {
  mat->matrix[0][0] = cos(angle);
  mat->matrix[0][1] = sin(angle);
  mat->matrix[1][0] = -1 * sin(angle);
  mat->matrix[1][1] = cos(angle);
  mat->matrix[2][2] = 1;
}

// Инициализация матрицы
matrix_t create_matrix(int rows, int columns) {
  matrix_t neo = {0};
  if (rows && columns) {
    neo.rows = rows;
    neo.columns = columns;
    neo.matrix = (double **)calloc(rows, sizeof(double *));
    for (int i = 0; i < rows; i++) {
      neo.matrix[i] = (double *)calloc((columns + 1), sizeof(double));
    }
    for (int r = 0; r < rows; r++) {
      for (int c = 0; c < columns; c++) {
        neo.matrix[r][c] = 0;
      }
    }
  }
  return neo;
}

// Очистка матриц
void remove_matrix(matrix_t *A) {
  if (A->columns) {
    for (int i = 0; i < A->rows && A->matrix[i] != NULL; i++) {
      free((double *)A->matrix[i]);
    }
  }
  if (A->matrix && A->columns) free((double **)A->matrix);
  A->columns = 0;
  A->rows = 0;
}

// Умножение матрицы на число
matrix_t mult_number(matrix_t *A, double number) {
  matrix_t muln = {0};
  muln = create_matrix(A->rows, A->columns);
  for (int r = 0; r < A->columns; r++) {
    for (int c = 0; c < A->rows; c++) {
      muln.matrix[c][r] = A->matrix[c][r] * number;
    }
  }
  return muln;
}

// Умножение матрицы на матрицу
matrix_t mult_matrix(matrix_t *A, matrix_t *B) {
  matrix_t ret = create_matrix(A->rows, B->columns);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      for (int k = 0; k < A->columns; k++) {
        ret.matrix[i][j] =
            ret.matrix[i][j] + (A->matrix[i][k] * B->matrix[k][j]);
      }
    }
  }
  return ret;
}

/* Сравнение матриц
@return SUCCESS 1
        FAILURE 0
*/
int eq_matrix(matrix_t *A, matrix_t *B) {
  int flag = 1;
  for (int r = 0; r < A->columns; r++) {
    for (int c = 0; c < A->rows; c++) {
      flag &= (fabs(A->matrix[c][r] - B->matrix[c][r]) < 1e-6);
    }
  }
  return flag;
}

/** Выводит на экран матрицу
 */
void print_matrix(matrix_t neo) {
  printf("Wake up, Neo. The matrix has you\n");
  for (int c = 0; c < neo.rows; c++) {
    for (int r = 0; r < neo.columns; r++) {
      printf("%f ", neo.matrix[c][r]);
      // printf("a->matrix[%d][%d] = %f;\n", c, r, neo.matrix[c][r]);
    }
    printf("\n");
  }
}
