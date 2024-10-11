#include "platform_info.h"
#include "ervp_printf.h"
#include "ervp_printf_section.h"
#include "ervp_variable_allocation.h"
#include "core_dependent.h"
#include "ervp_malloc.h"
#include "ervp_delay.h"
#include "ervp_assert.h"

#include "ervp_matrix.h"
#include "ervp_matrix_op.h"
#include "ervp_matrix_op_sw.h"
#include "ervp_special_matrix_op.h"
#include "ervp_blocked_matrix_op.h"

#include "vta_api.h"
#include "test_matrix.h"

// this app is modified from "verify_matrix_opt"

#define BLOCK_SIZE 16

static void blocked_matrix_mult_vta(const ErvpMatrixInfo* a, const ErvpMatrixInfo* b, ErvpMatrixInfo* c)
{
  matrix_zero_opt(c);
  blocked_matrix_mac(a,b,c,BLOCK_SIZE);
}

static void submatrix_mult_vta(const ErvpMatrixInfo* a, const ErvpMatrixInfo* b, ErvpMatrixInfo* c)
{
  if( (a->num_row == BLOCK_SIZE) && (a->num_col == BLOCK_SIZE) 
      && (b->num_row == BLOCK_SIZE) && (b->num_col == BLOCK_SIZE))
  {
    matrix_mult_vta_16x16(a,b,c);
  }
  else
  {
    matrix_mult_sw(a,b,c);
  }
}

static inline void register_opt_function()
{
  matrix_mult_opt = blocked_matrix_mult_vta;
  submatrix_mult_opt = submatrix_mult_vta;
  //submatrix_mult_opt = matrix_mult_sw;
  submatrix_mac_opt = NULL;
}

static char hw_name[] = "BLOCKED_VTA";

///////////////////////////////////////////////////////////////

#define NUN_MATRIX 1
#define TEST_MATRIX_SIZE 63 //// overflow?????????

#define VERIFY_ADD 0
#define VERIFY_SUB 0
#define VERIFY_EWMULT 0
#define VERIFY_MULT 1

#define ML_DATATYPE MATRIX_DATATYPE_UINT08
#define MR_DATATYPE MATRIX_DATATYPE_UINT08
#define MO_DATATYPE MATRIX_DATATYPE_UINT08

#if (GET_NUM_BITS(ML_DATATYPE)==32)
typedef uint32_t ml_type_t;
#elif (GET_NUM_BITS(ML_DATATYPE)==16)
typedef uint16_t ml_type_t;
#else
typedef uint8_t ml_type_t;
#endif

#if (GET_NUM_BITS(MR_DATATYPE)==32)
typedef uint32_t mr_type_t;
#elif (GET_NUM_BITS(MR_DATATYPE)==16)
typedef uint16_t mr_type_t;
#else
typedef uint8_t mr_type_t;
#endif

#if (GET_NUM_BITS(MO_DATATYPE)==32)
typedef uint32_t mo_type_t;
#elif (GET_NUM_BITS(MO_DATATYPE)==16)
typedef uint16_t mo_type_t;
#else
typedef uint8_t mo_type_t;
#endif

#define SKIP_SIM 1

#define ALIGNED_MATRIX __attribute__ ((aligned(0x1000)))

ml_type_t input_left_array[NUN_MATRIX][TEST_MATRIX_SIZE][TEST_MATRIX_SIZE] BIG_DATA_BSS ALIGNED_MATRIX;
mr_type_t input_right_array[NUN_MATRIX][TEST_MATRIX_SIZE][TEST_MATRIX_SIZE] BIG_DATA_BSS ALIGNED_MATRIX;
mo_type_t output_array[NUN_MATRIX][TEST_MATRIX_SIZE][TEST_MATRIX_SIZE] BIG_DATA_BSS ALIGNED_MATRIX;
mo_type_t ref_array[NUN_MATRIX][TEST_MATRIX_SIZE][TEST_MATRIX_SIZE] BIG_DATA_BSS ALIGNED_MATRIX;

ErvpMatrixInfo* input_left_info = NULL;
ErvpMatrixInfo* input_right_info = NULL;
ErvpMatrixInfo* output_info = NULL;
ErvpMatrixInfo* ref_info = NULL;

void matrix_info_init()
{
  input_left_info = matrix_generate_info(ML_DATATYPE,TEST_MATRIX_SIZE,TEST_MATRIX_SIZE,NULL,NULL);
  input_right_info = matrix_generate_info(MR_DATATYPE,TEST_MATRIX_SIZE,TEST_MATRIX_SIZE,NULL,NULL);
  output_info = matrix_generate_info(MO_DATATYPE,TEST_MATRIX_SIZE,TEST_MATRIX_SIZE,NULL,NULL);
  ref_info = matrix_generate_info(MO_DATATYPE,TEST_MATRIX_SIZE,TEST_MATRIX_SIZE,NULL,NULL);
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
  int all_are_equal;

  // init
  matrix_info_init();
  register_opt_function();

  // init matrices
  for(int i=0; i<NUN_MATRIX; i=i+1)
  {
    matrix_info_setup(i);
    generate_test_matrix(input_left_info, i);
    //generate_test_matrix(input_right_info, i+1);
  }

#if 1
  // init right matrices as identity matrix
  for(int i=0; i<NUN_MATRIX; i=i+1)
  {
    // can change this part to test ordinary matrices
    for(int j=0; j<TEST_MATRIX_SIZE; j=j+1)
      for(int k=0; k<TEST_MATRIX_SIZE; k=k+1)
        input_right_array[i][j][k] = (j==k);
  }
#endif
  flush_cache();
  
  //
  if(VERIFY_MULT)
  {
    printf_section(SKIP_SIM, "%s_MULT", hw_name);
    for(int i=0; i<NUN_MATRIX; i=i+1)
    {
      matrix_info_setup(i);
      matrix_mult_sw(input_left_info, input_right_info, ref_info);
      matrix_mult_opt(input_left_info, input_right_info, output_info);
      all_are_equal = matrix_compare(output_info, ref_info, 1);
      if(!all_are_equal)
      {
        matrix_print(input_left_info);
        matrix_print(input_right_info);
        matrix_print(output_info);
        matrix_print(ref_info);
        break;
      }
    }
  }
}
