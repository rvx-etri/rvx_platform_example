#include "platform_info.h"
#include "ervp_printf.h"
#include "ervp_variable_allocation.h"
#include "test_matrix.h"

#define NUN_MATRIX 1
#define TEST_MATRIX_SIZE 8
#define RESULT_CHECK 1

#define MATRIX_DATATYPE MATRIX_DATATYPE_SINT08
//#define MATRIX_TYPE MATRIX_DATATYPE_FLOAT32

#if (MATRIX_DATATYPE==MATRIX_DATATYPE_SINT08)
typedef int8_t MTYPE;
#endif
#if (MATRIX_DATATYPE==MATRIX_DATATYPE_UINT08)
typedef uint8_t MTYPE;
#endif
#if (MATRIX_DATATYPE==MATRIX_DATATYPE_FLOAT32)
typedef float MTYPE;
#endif

#define ALIGNED_MATRIX __attribute__ ((aligned(0x1000)))

MTYPE input_left_array[NUN_MATRIX][TEST_MATRIX_SIZE][TEST_MATRIX_SIZE] BIG_DATA_BSS ALIGNED_MATRIX;
MTYPE input_right_array[NUN_MATRIX][TEST_MATRIX_SIZE][TEST_MATRIX_SIZE] BIG_DATA_BSS ALIGNED_MATRIX;
MTYPE output_array[NUN_MATRIX][TEST_MATRIX_SIZE][TEST_MATRIX_SIZE] BIG_DATA_BSS ALIGNED_MATRIX;
MTYPE ref_array[NUN_MATRIX][TEST_MATRIX_SIZE][TEST_MATRIX_SIZE] BIG_DATA_BSS ALIGNED_MATRIX;

ErvpMatrixInfo* input_left_info = NULL;
ErvpMatrixInfo* input_right_info = NULL;
ErvpMatrixInfo* output_info = NULL;
ErvpMatrixInfo* ref_info = NULL;

static inline void test_matrix_mult(const ErvpMatrixInfo* a, const ErvpMatrixInfo* b, ErvpMatrixInfo* c)
{
  // update this function to test your hardware
  matrix_mult_sw(a, b, c);
}

void matrix_info_init()
{
  input_left_info = matrix_generate_info(MATRIX_DATATYPE,TEST_MATRIX_SIZE,TEST_MATRIX_SIZE,NULL,NULL);
  input_right_info = matrix_generate_info(MATRIX_DATATYPE,TEST_MATRIX_SIZE,TEST_MATRIX_SIZE,NULL,NULL);
  output_info = matrix_generate_info(MATRIX_DATATYPE,TEST_MATRIX_SIZE,TEST_MATRIX_SIZE,NULL,NULL);
  ref_info = matrix_generate_info(MATRIX_DATATYPE,TEST_MATRIX_SIZE,TEST_MATRIX_SIZE,NULL,NULL);
}

void matrix_info_setup(int index)
{
  input_left_info->addr = (void*)(input_left_array[index]);
  input_right_info->addr = (void*)(input_right_array[index]);
  output_info->addr = (void*)(output_array[index]);
  ref_info->addr = (void*)(ref_array[index]);
}

int main()
{
  // init
  matrix_info_init();

  // init left matrices
  for(int i=0; i<NUN_MATRIX; i=i+1)
  {
    matrix_info_setup(i);
    generate_test_matrix(input_left_info, i);
  }

  // init right matrices as identity matrix
  for(int i=0; i<NUN_MATRIX; i=i+1)
  {
    matrix_info_setup(i);
    // can change this part to test ordinary matrices
    for(int j=0; j<TEST_MATRIX_SIZE; j=j+1)
      for(int k=0; k<TEST_MATRIX_SIZE; k=k+1)
        input_right_array[i][j][k] = (j==k);
  }

  // ref matrices equals to the left due to the identity matrix
  //for(int i=0; i<NUN_MATRIX; i=i+1)
  //{
  //  matrix_info_setup(i);
  //  matrix_mult_sw(&input_left_info, &input_right_info, &ref_info);
  //}

  // verify
  for(int i=0; i<NUN_MATRIX; i=i+1)
  {
    matrix_info_setup(i);
    ref_info = input_left_info; // REMOVE if you change the right matrices
    test_matrix_mult(input_left_info, input_right_info, output_info);
    if(RESULT_CHECK)
      matrix_compare(output_info, ref_info, 1);
  }
	return 0;
}
