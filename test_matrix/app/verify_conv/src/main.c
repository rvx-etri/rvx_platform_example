#include "platform_info.h"
#include "ervp_printf.h"
#include "ervp_multicore_synch.h"

#include "ervp_matrix.h"
#include "ervp_matrix_op.h"
#include "ervp_special_matrix.h"

#include "test_matrix.h"

#define NUN_MATRIX 5
#define INPUT_MATRIX_SIZE 31
#define KERNEL_MATRIX_SIZE 3
#define RESULT_CHECK 1

#define MATRIX_DATATYPE MATRIX_DATATYPE_SINT32
//#define MATRIX_DATATYPE MATRIX_DATATYPE_FLOAT32

ErvpMatrixInfo* input_info = NULL;
ErvpMatrixInfo* kernel_info = NULL;
ErvpMatrixInfo* output_info = NULL;
ErvpMatrixInfo* ref_info = NULL;

int main() {
	printf("Matrix Convolution Test!\n");

  ervp_conv_option_t conv_option;
  conv_option.value = 0;
  conv_option.br.pretty_to_mult = 16;
  conv_option.br.boundary_to_mult = 1;

  input_info = matrix_alloc(MATRIX_DATATYPE, INPUT_MATRIX_SIZE, INPUT_MATRIX_SIZE, NULL);
  kernel_info = matrix_alloc(MATRIX_DATATYPE, KERNEL_MATRIX_SIZE, KERNEL_MATRIX_SIZE, NULL);
  output_info = matrix_alloc_conv_output(input_info, kernel_info, conv_option.value);
  ref_info = matrix_alloc_conv_output(input_info, kernel_info, conv_option.value);

  matrix_conv_opt = matrix_conv_by_mult;

  for(int i=0; i<NUN_MATRIX; i=i+1)
  {
    // generate input
    generate_test_matrix(input_info, i);

    // generate kernal and ref
    if(KERNEL_MATRIX_SIZE==1)
    {
      matrix_one(kernel_info);
      ref_info = input_info;
    }
    else
    {
      generate_test_matrix(kernel_info, i);
      matrix_conv_sw(input_info, kernel_info, ref_info, conv_option.value);
    }

    matrix_conv_opt(input_info, kernel_info, output_info, conv_option.value);
    
    if(RESULT_CHECK)
      matrix_compare(output_info, ref_info, 1);
    //matrix_print(input_info);
    //matrix_print(kernel_info);
    //matrix_print(output_info);
  }

  return 0;
}
