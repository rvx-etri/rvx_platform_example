#include "platform_info.h"
#include "ervp_printf.h"
#include "ervp_printf_section.h"
#include "ervp_variable_allocation.h"
#include "ervp_special_matrix_op.h"
#include "core_dependent.h"
#include "ervp_matrix.h"
#include "ervp_matrix_op_sw.h"
#include "ervp_blocked_matrix_op.h"
#include "ervp_core_id.h"
#include "ervp_assert.h"

#include "test_matrix.h"

// this app is modified from "verify_matrix_opt"

#define BLOCK_SIZE 4

static inline void register_matrix_function(ervp_mop_mapping_t* mop_mapping)
{
  ervp_blocked_matrix_info_t* blocked_info = blocked_matrix_info_alloc();
  blocked_info->block_size = BLOCK_SIZE;
  blocked_info->subop_mapping = matrix_op_mapping_alloc();
  matrix_op_register_blocked_global(mop_mapping, blocked_info);
}

static char hw_name[] = "BLOCKED_SW";

///////////////////////////////////////////////////////////////

#define NUN_MATRIX 1
#define TEST_MATRIX_SIZE 6

#define VERIFY_ADD 1
#define VERIFY_SUB 1
#define VERIFY_EWMULT 1
#define VERIFY_MULT 1

#define ML_DATATYPE MATRIX_DATATYPE_SINT08
#define MR_DATATYPE MATRIX_DATATYPE_SINT08
#define MO_DATATYPE MATRIX_DATATYPE_SINT08

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
  if(EXCLUSIVE_ID==0)
  {
    ervp_mop_mapping_t* mop_mapping = matrix_op_mapping_alloc();
    register_matrix_function(mop_mapping);
    matrix_info_init();

    // init matrices
    for(int i=0; i<NUN_MATRIX; i=i+1)
    {
      matrix_info_setup(i);
      generate_test_matrix(input_left_info, i);
      generate_test_matrix(input_right_info, i+1);
    }
    flush_cache();

    //
    if(VERIFY_ADD)
    {
      printf_section(SKIP_SIM, "%s_ADD", hw_name);
      for(int i=0; i<NUN_MATRIX; i=i+1)
      {
        flush_cache();
        matrix_info_setup(i);
        matrix_add_sw(input_left_info, input_right_info, ref_info, 0);
        mop_mapping->matrix_add(mop_mapping, input_left_info, input_right_info, output_info, 0);
        int all_are_equal = matrix_compare(output_info, ref_info, 1);
        if(!all_are_equal)
        {
          matrix_print(input_left_info);
          matrix_print(input_right_info);
          matrix_print(output_info);
          matrix_print(ref_info);
          assert(0);
          break;
        }
      }
    }
    
    //
    if(VERIFY_SUB)
    {
      printf_section(SKIP_SIM, "%s_SUB", hw_name);
      for(int i=0; i<NUN_MATRIX; i=i+1)
      {
        flush_cache();
        matrix_info_setup(i);
        matrix_sub_sw(input_left_info, input_right_info, ref_info, 0);
        mop_mapping->matrix_sub(mop_mapping, input_left_info, input_right_info, output_info, 0);
        int all_are_equal = matrix_compare(output_info, ref_info, 1);
        if(!all_are_equal)
        {
          matrix_print(input_left_info);
          matrix_print(input_right_info);
          matrix_print(output_info);
          matrix_print(ref_info);
          assert(0);
          break;
        }
      }
    }
    
    //
    if(VERIFY_EWMULT)
    {
      printf_section(SKIP_SIM, "%s_EWMULT", hw_name);
      for(int i=0; i<NUN_MATRIX; i=i+1)
      {
        flush_cache();
        matrix_info_setup(i);
        matrix_ewmult_sw(input_left_info, input_right_info, ref_info, 0);
        mop_mapping->matrix_ewmult(mop_mapping, input_left_info, input_right_info, output_info, 0);
        int all_are_equal = matrix_compare(output_info, ref_info, 1);
        if(!all_are_equal)
        {
          matrix_print(input_left_info);
          matrix_print(input_right_info);
          matrix_print(output_info);
          matrix_print(ref_info);
          assert(0);
          break;
        }
      }
    }
    
    //
    if(VERIFY_MULT)
    {
      printf_section(SKIP_SIM, "%s_MULT", hw_name);
      for(int i=0; i<NUN_MATRIX; i=i+1)
      {
        flush_cache();
        matrix_info_setup(i);
        matrix_mult_sw(input_left_info, input_right_info, ref_info, 0);
        mop_mapping->matrix_mult(mop_mapping, input_left_info, input_right_info, output_info, 0);
        int all_are_equal = matrix_compare(output_info, ref_info, 1);
        if(!all_are_equal)
        {
          matrix_print(input_left_info);
          matrix_print(input_right_info);
          matrix_print(output_info);
          matrix_print(ref_info);
          assert(0);
          break;
        }
      }
    }

    flush_cache();
  }
  return 0;
}
