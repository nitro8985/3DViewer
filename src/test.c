#include "test.h"

START_TEST(cube_matrix) {
  matrix_t matrix;
  data_t object;
  fill_cube_matrix(&matrix);
  char fname[] = "obj/cube.obj";
  if (parse(fname, &object) == 0) {
    ck_assert_int_eq(eq_matrix(&matrix, &object.matrix_3d), 1);
  }
  remove_matrix(&matrix);
  clean_obj(&object);
}
END_TEST

START_TEST(rotate_matrix) {
  matrix_t matrix;
  data_t object;
  fill_rotate_matrix(&matrix);
  char fname[] = "obj/cube.obj";
  if (parse(fname, &object) == 0) {
    matrix_t rotx = create_matrix(3, 3);
    matrix_t roty = create_matrix(3, 3);
    matrix_t rotz = create_matrix(3, 3);
    set_rot_matrix_x(&rotx, 5 * GR2RAD);
    set_rot_matrix_y(&roty, 10 * GR2RAD);
    set_rot_matrix_z(&rotz, 15 * GR2RAD);
    matrix_t mat1 = mult_matrix(&rotx, &object.matrix_3d);
    matrix_t mat2 = mult_matrix(&roty, &mat1);
    matrix_t mat3 = mult_matrix(&rotz, &mat2);
    remove_matrix(&rotx);
    remove_matrix(&roty);
    remove_matrix(&rotz);
    // print_matrix(object.matrix_3d);
    // print_matrix(matrix);
    ck_assert_int_eq(eq_matrix(&matrix, &mat3), 1);
    remove_matrix(&mat1);
    remove_matrix(&mat2);
    remove_matrix(&mat3);
  }
  remove_matrix(&matrix);
  clean_obj(&object);
}
END_TEST

int main(void) {
  int problems_count = 0;
  Suite *main_suite;
  SRunner *runner;

  main_suite = suite_create("All_tests");
  suite_add_tcase(main_suite, matrix_tests());
  runner = srunner_create(main_suite);

  srunner_run_all(runner, CK_VERBOSE);
  problems_count = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (problems_count == 0) ? 0 : 1;
}

TCase *matrix_tests(void) {
  TCase *t_case = tcase_create("viever_functions");

  tcase_add_test(t_case, cube_matrix);
  tcase_add_test(t_case, rotate_matrix);

  return t_case;
}

void fill_rotate_matrix(matrix_t *a) {
  *a = create_matrix(3, 9);
  a->matrix[0][0] = 0.000000;
  a->matrix[0][1] = -0.539584;
  a->matrix[0][2] = 0.411667;
  a->matrix[0][3] = -0.267131;
  a->matrix[0][4] = 0.684120;
  a->matrix[0][5] = -0.411667;
  a->matrix[0][6] = 0.539584;
  a->matrix[0][7] = -0.684120;
  a->matrix[0][8] = 0.267131;

  a->matrix[1][0] = 0.000000;
  a->matrix[1][1] = -0.416202;
  a->matrix[1][2] = -0.671089;
  a->matrix[1][3] = 0.542131;
  a->matrix[1][4] = 0.287244;
  a->matrix[1][5] = 0.671089;
  a->matrix[1][6] = 0.416202;
  a->matrix[1][7] = -0.287244;
  a->matrix[1][8] = -0.542131;

  a->matrix[2][0] = 0.000000;
  a->matrix[2][1] = 0.534438;
  a->matrix[2][2] = 0.360790;
  a->matrix[2][3] = 0.620270;
  a->matrix[2][4] = 0.446622;
  a->matrix[2][5] = -0.360790;
  a->matrix[2][6] = -0.534438;
  a->matrix[2][7] = -0.446622;
  a->matrix[2][8] = -0.620270;
}

void fill_cube_matrix(matrix_t *a) {
  *a = create_matrix(3, 9);
  a->matrix[0][0] = 0.000000;
  a->matrix[0][1] = -0.500000;
  a->matrix[0][2] = 0.500000;
  a->matrix[0][3] = -0.500000;
  a->matrix[0][4] = 0.500000;
  a->matrix[0][5] = -0.500000;
  a->matrix[0][6] = 0.500000;
  a->matrix[0][7] = -0.500000;
  a->matrix[0][8] = 0.500000;

  a->matrix[1][0] = 0.000000;
  a->matrix[1][1] = -0.500000;
  a->matrix[1][2] = -0.500000;
  a->matrix[1][3] = 0.500000;
  a->matrix[1][4] = 0.500000;
  a->matrix[1][5] = 0.500000;
  a->matrix[1][6] = 0.500000;
  a->matrix[1][7] = -0.500000;
  a->matrix[1][8] = -0.500000;

  a->matrix[2][0] = 0.000000;
  a->matrix[2][1] = 0.500000;
  a->matrix[2][2] = 0.500000;
  a->matrix[2][3] = 0.500000;
  a->matrix[2][4] = 0.500000;
  a->matrix[2][5] = -0.500000;
  a->matrix[2][6] = -0.500000;
  a->matrix[2][7] = -0.500000;
  a->matrix[2][8] = -0.500000;
}
