#define _GNU_SOURCE
#include "parse.h"

// int main() {
//   char fname[] = "obj/cube.obj";
//   data_t object;
//   if (parse(fname, &object) == 0) {
//     print_data(object);
//     clean_obj(&object);
//   } else {
//     printf("error opening file %s", fname);
//   }
//   return 0;
// }

// Чистим память
void clean_obj(data_t *object) {
  for (int i = 0; i < object->count_of_facets; i++) {
    free(object->polygons[i].vertexes);
  }
  free(object->polygons);
  remove_matrix(&(object->matrix_3d));
  object->count_of_facets = 0;
  object->count_of_vertexes = 0;
}

// Парсинг файла
int parse(char *fname, data_t *object) {
  FILE *fl = fopen(fname, "r");
  int err = 0;
  object->count_of_facets = 0;
  object->count_of_vertexes = 0;
  if (fl) {
    count_v_f(fl, object);
    get_v_f(fl, object);
    fclose(fl);
  } else {
    err = 1;
  }
  return err;
}

// Считает количество пробелов в строке
int count_spaces(char *str) {
  int spaces = 0;
  while (*str++) {
    if (*str == ' ') spaces++;
  }
  return spaces;
}

// Парсинг токена facets - берет только первое число
int get_facet(char *str) {
  char facet[11];
  int i = 0;
  while (*str != '/' && *str != ' ' && *str != '\0') {
    facet[i] = *str;
    i++;
    str++;
  }
  facet[i] = '\0';
  return atoi(facet);
}

// Вывод на экран точек и фасетов
void print_data(data_t object) {
  printf("vertex = %d\nfacets = %d\n", object.count_of_vertexes,
         object.count_of_facets);
  for (int j = 1; j < object.count_of_vertexes + 1; j++) {
    printf("x=%lf y=%lf z=%lf\n", object.matrix_3d.matrix[0][j],
           object.matrix_3d.matrix[1][j], object.matrix_3d.matrix[2][j]);
  }
  for (int i = 0; i < object.count_of_facets; i++) {
    // printf("facet %d - ", i);
    for (int j = 0; j < object.polygons[i].numbers_of_vertexes_in_facets; j++) {
      printf("a->matrix[%d][%d] = %d;\n", i, j, object.polygons[i].vertexes[j]);
      //   printf("%d ", object.polygons[i].vertexes[j]);
    }
    printf("\n");
  }
}

// Проходит по файлу и считает количество строк v и f
void count_v_f(FILE *fl, data_t *object) {
  size_t fbufsize = 255;
  char *fstr = (char *)calloc(fbufsize, sizeof(char));
  while (getline(&fstr, &fbufsize, fl) != -1) {
    if (fstr[0] == 'v' && fstr[1] == ' ') object->count_of_vertexes++;
    if (fstr[0] == 'f' && fstr[1] == ' ') object->count_of_facets++;
  }
  object->matrix_3d = create_matrix(3, object->count_of_vertexes + 1);
  if (fstr) free(fstr);
}

// Второй раз проходит по файлу и забирает данные в структуру
void get_v_f(FILE *fl, data_t *object) {
  object->polygons = calloc(object->count_of_facets, sizeof(polygon_t));
  fseek(fl, 0, SEEK_SET);
  size_t fbufsize = 255;
  int v = 1;
  int f = 0;
  double maxvalue = 0.;
  char *fstr = (char *)calloc(fbufsize, sizeof(char));
  while (getline(&fstr, &fbufsize, fl) != -1) {
    if (fstr[0] == 'v' && fstr[1] == ' ') {
      char id;
      double x, y, z;
      sscanf(fstr, "%c%lf%lf%lf", &id, &x, &y, &z);
      maxvalue = getmax(maxvalue, x, y, z);
      object->matrix_3d.matrix[0][v] = x;
      object->matrix_3d.matrix[1][v] = y;
      object->matrix_3d.matrix[2][v] = z;
      v++;
    }
    if (fstr[0] == 'f' && fstr[1] == ' ') {
      object->polygons[f].numbers_of_vertexes_in_facets = count_spaces(fstr);
      object->polygons[f].vertexes = calloc(
          object->polygons[f].numbers_of_vertexes_in_facets, sizeof(int));
      char *leksem = strtok(fstr, " ");
      int j = 0;
      while (leksem != NULL) {
        leksem = strtok(NULL, " ");
        if (leksem) {
          object->polygons[f].vertexes[j] = get_facet(leksem);
          j++;
        }
      }
      f++;
    }
  }
  double resize = 1;
  while (maxvalue > 10) {
    resize /= 10;
    maxvalue /= 10;
  }

  matrix_t tmp = mult_number(&object->matrix_3d, resize);
  remove_matrix(&object->matrix_3d);
  object->matrix_3d = tmp;
  if (fstr) free(fstr);
}

// Возвращает максимальное значение координат
double getmax(double num, double x, double y, double z) {
  double res = 0.0;
  if (x > res) res = x;
  if (y > res) res = y;
  if (z > res) res = z;
  return (res > num) ? res : num;
}
