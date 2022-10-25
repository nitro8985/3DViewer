#ifndef SRC_PARSE_H_
#define SRC_PARSE_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"

#define N 250

typedef struct facets {
  int *vertexes;
  int numbers_of_vertexes_in_facets;
} polygon_t;

typedef struct data {
  int count_of_vertexes;
  int count_of_facets;
  matrix_t matrix_3d;
  polygon_t *polygons;
} data_t;

int count_spaces(char *str);
int get_facet(char *str);
void print_data(data_t object);
void count_v_f(FILE *fl, data_t *object);
void get_v_f(FILE *fl, data_t *object);
int parse(char *fname, data_t *object);
void clean_obj(data_t *object);
double getmax(double num, double x, double y, double z);

#endif  // SRC_PARSE_H_
