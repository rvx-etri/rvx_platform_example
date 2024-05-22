#ifndef __TEST_MATRIX_H__
#define __TEST_MATRIX_H__

#include "ervp_matrix.h"

#define LARGE_MATRIX_SIZE 32

extern char int08_large_array[LARGE_MATRIX_SIZE][LARGE_MATRIX_SIZE];
extern short int16_large_array[LARGE_MATRIX_SIZE][LARGE_MATRIX_SIZE];
extern float float_large_array[LARGE_MATRIX_SIZE][LARGE_MATRIX_SIZE];

extern ErvpMatrixInfo int08_large_info;
extern ErvpMatrixInfo int16_large_info;
extern ErvpMatrixInfo float_large_info;

void generate_test_matrix(ErvpMatrixInfo* matrix_info, int index);
void init_large_array_int08(int index);
void init_large_array_int16(int index);
void init_large_array_float(int index);

#endif
