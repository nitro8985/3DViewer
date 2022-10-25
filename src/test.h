#ifndef SRC_TEST_H_
#define SRC_TEST_H_

#include <check.h>
#include <limits.h>
#include <stdlib.h>

#include "parse.h"

#define GR2RAD 0.0174533

#define DESTROY(VALUE) \
  if (VALUE) free(VALUE)

TCase *matrix_tests(void);
void fill_cube_matrix(matrix_t *a);
void fill_rotate_matrix(matrix_t *a);

#endif  // SRC_TEST_H_
