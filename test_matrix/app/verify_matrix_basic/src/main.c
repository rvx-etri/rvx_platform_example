#include "platform_info.h"
#include "ervp_printf.h"
#include "ervp_variable_allocation.h"
#include "ervp_matrix_op.h"
#include "ervp_special_matrix_op.h"
#include "ervp_core_id.h"
#include "ervp_assert.h"

#include "test_matrix.h"

#define NUN_MATRIX 1
#define TEST_MATRIX_SIZE 8
#define RESULT_CHECK 1

#define MATRIX_DATATYPE MATRIX_DATATYPE_SINT08
//#define MATRIX_DATATYPE MATRIX_DATATYPE_FLOAT32

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

void matrix_info_init()
{
  input_left_info = matrix_generate_info(MATRIX_DATATYPE,TEST_MATRIX_SIZE,TEST_MATRIX_SIZE,NULL,NULL);
  input_right_info = matrix_generate_info(MATRIX_DATATYPE,TEST_MATRIX_SIZE,TEST_MATRIX_SIZE,NULL,NULL);
  output_info = matrix_generate_info(MATRIX_DATATYPE,TEST_MATRIX_SIZE,TEST_MATRIX_SIZE,NULL,NULL);
}

void matrix_info_setup(int index)
{
  input_left_info->addr = (void*)(input_left_array[index]);
  input_right_info->addr = (void*)(input_right_array[index]);
  output_info->addr = (void*)(output_array[index]);
}

int main()
{
  if(EXCLUSIVE_ID==0)
  {
    // init
    matrix_op_check();
    matrix_info_init();
    for(int i=0; i<NUN_MATRIX; i=i+1)
    {
      // init matrices
      matrix_info_setup(i);
      generate_test_matrix(input_left_info, i);
      // add
      matrix_zero_opt(input_right_info);
      matrix_zero_opt(output_info);
      matrix_add_opt(input_left_info, input_right_info, output_info, 0);
      if(RESULT_CHECK)
      {
        int all_are_equal = matrix_compare(output_info, input_left_info, 1);
        assert(all_are_equal);
      }
      // sub
      matrix_zero_opt(input_right_info);
      matrix_zero_opt(output_info);
      matrix_sub_opt(input_left_info, input_right_info, output_info, 0);
      if(RESULT_CHECK)
      {
        int all_are_equal = matrix_compare(output_info, input_left_info, 1);
        assert(all_are_equal);
      }
      // ewmult
      matrix_one_opt(input_right_info);
      matrix_zero_opt(output_info);
      matrix_ewmult_opt(input_left_info, input_right_info, output_info, 0);
      if(RESULT_CHECK)
      {
        int all_are_equal = matrix_compare(output_info, input_left_info, 1);
        assert(all_are_equal);
      }
      // scalar mult
      matrix_one_opt(input_right_info);
      matrix_zero_opt(output_info);
      matrix_scalar_mult_fixed_opt(input_left_info, 1, output_info, 0);
      if(RESULT_CHECK)
      {
        int all_are_equal = matrix_compare(output_info, input_left_info, 1);
        assert(all_are_equal);
      }
      // mult
      matrix_identity_opt(input_right_info);
      matrix_zero_opt(output_info);
      matrix_mult_opt(input_left_info, input_right_info, output_info, 0);
      if(RESULT_CHECK)
      {
        int all_are_equal = matrix_compare(output_info, input_left_info, 1);
        assert(all_are_equal);
      }
    }
  }
  return 0;
}
