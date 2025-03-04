#include "platform_info.h"
#include "ervp_printf.h"
#include "ervp_printf_section.h"
#include "ervp_multicore_synch.h"
#include "ervp_core_id.h"

#include "ervp_matrix.h"
#include "ervp_matrix_op_sw.h"
#include "ervp_special_matrix_op.h"

#include "test_matrix.h"

#define NUN_MATRIX 1
#define INPUT_MATRIX_SIZE 8
#define KERNEL_MATRIX_SIZE 3
#define RESULT_CHECK 1

#define MATRIX_DATATYPE MATRIX_DATATYPE_SINT32
//#define MATRIX_DATATYPE MATRIX_DATATYPE_FLOAT32

ErvpMatrixInfo* input_info = NULL;
ErvpMatrixInfo* kernel_info = NULL;
ErvpMatrixInfo* output_info = NULL;
ErvpMatrixInfo* ref_info = NULL;

static inline void register_matrix_function(ervp_mop_mapping_t* mop_mapping)
{
  mop_mapping->matrix_conv = matrix_conv2mult_v1;
}

int main()
{
  if(EXCLUSIVE_ID==0)
  {
    ervp_mop_mapping_t* mop_mapping = matrix_op_mapping_alloc();
    register_matrix_function(mop_mapping);

    printf_section(1, "CONV");

    ervp_mconv_option_t conv_option;
    conv_option.value = 0;
    conv_option.br.pretty_to_mult = 16;
    conv_option.br.boundary_to_mult = 0;
    conv_option.br.rshift = 1;

    input_info = matrix_alloc(MATRIX_DATATYPE, INPUT_MATRIX_SIZE, INPUT_MATRIX_SIZE, NULL);
    kernel_info = matrix_alloc(MATRIX_DATATYPE, KERNEL_MATRIX_SIZE, KERNEL_MATRIX_SIZE, NULL);
    output_info = matrix_conv_alloc_output(input_info, kernel_info, conv_option.value);
    ref_info = matrix_conv_alloc_output(input_info, kernel_info, conv_option.value);

    for(int i=0; i<NUN_MATRIX; i=i+1)
    {
      // generate input
      generate_test_matrix(input_info, i);

      // generate kernal and ref
      if(KERNEL_MATRIX_SIZE==1)
      {
        matrix_one_sw(kernel_info);
        ref_info = input_info;
      }
      else
      {
        generate_test_matrix(kernel_info, i);
        matrix_conv_sw(input_info, kernel_info, ref_info, conv_option.value);
      }

      mop_mapping->matrix_conv(mop_mapping, input_info, kernel_info, output_info, conv_option.value);
      
      if(RESULT_CHECK)
      {
        int all_are_equal = matrix_compare(output_info, ref_info, 1);
        if(!all_are_equal)
        {
          matrix_print(input_info);
          matrix_print(kernel_info);
          matrix_print(output_info);
          matrix_print(ref_info);
          break;
        }
      }
    }
  }

  return 0;
}
