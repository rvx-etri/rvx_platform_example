#include "platform_info.h"
#include "ervp_platform_controller_api.h"
#include "ervp_printf.h"
#include "ervp_printf_section.h"
#include "ervp_variable_allocation.h"

#include "ervp_matrix_element.h"
#include "ervp_special_matrix_op.h"


#define MATRIX_SIZE 4
#define RESULT_CHECK 1

int32_t matrix_test[MATRIX_SIZE][MATRIX_SIZE];
int32_t matrix_ref0[4] = {0x87654321, 0x98765432, 0xa9876543, 0xb987654};
int16_t matrix_ref1[4] = {0x4321, 0x5432, 0x6543, 0x7654};
int8_t matrix_ref3[4] = {0x21, 0x84};

ErvpMatrixInfo* matrix_test_info = NULL;
ErvpMatrixInfo* matrix_ref0_info = NULL;
ErvpMatrixInfo* matrix_ref1_info = NULL;
ErvpMatrixInfo* matrix_ref2_info = NULL;
ErvpMatrixInfo* matrix_ref3_info = NULL;

#define SKIP_SIM 1

void matrix_info_init()
{
  matrix_ref0_info = matrix_generate_info(MATRIX_DATATYPE_UINT08,MATRIX_SIZE,MATRIX_SIZE,matrix_ref0, NULL);
  matrix_ref1_info = matrix_generate_info(MATRIX_DATATYPE_UINT04,MATRIX_SIZE,MATRIX_SIZE,matrix_ref1, NULL);
  matrix_ref2_info = matrix_alloc(MATRIX_DATATYPE_UINT08,MATRIX_SIZE,MATRIX_SIZE,NULL);
  matrix_ref3_info = matrix_generate_info(MATRIX_DATATYPE_UINT01,MATRIX_SIZE,MATRIX_SIZE,matrix_ref3, NULL);
}

int main()
{
  matrix_info_init();
  matrix_identity_sw(matrix_ref2_info);
  //
  printf_section(SKIP_SIM, "subbyte matrix print");
  matrix_print_hex(matrix_ref0_info);
  if(!is_sim())
  {
    matrix_print(matrix_ref1_info);
    matrix_print(matrix_ref2_info);
    matrix_print(matrix_ref3_info);
  }

  //
  printf_section(SKIP_SIM, "subbyte matrix test 0");
  matrix_test_info = matrix_generate_info(MATRIX_DATATYPE_UINT01,MATRIX_SIZE,MATRIX_SIZE,matrix_test,NULL);
  matrix_identity_sw(matrix_test_info);
  if(!is_sim())
  {
    matrix_print(matrix_test_info);
    matrix_print(matrix_ref2_info);
  }
  matrix_compare(matrix_test_info,matrix_ref2_info,1);

  //
  printf_section(SKIP_SIM, "subbyte matrix test 1");
  matrix_test_info = matrix_generate_info(MATRIX_DATATYPE_SINT02,MATRIX_SIZE,MATRIX_SIZE,matrix_test,NULL);
  matrix_identity_sw(matrix_test_info);
  if(!is_sim())
  {
    matrix_print(matrix_test_info);
    matrix_print(matrix_ref2_info);
  }
  matrix_compare(matrix_test_info,matrix_ref2_info,1);

  //
  printf_section(SKIP_SIM, "subbyte matrix test 2");
  matrix_test_info = matrix_generate_info(MATRIX_DATATYPE_UINT04,MATRIX_SIZE,MATRIX_SIZE,matrix_test,NULL);
  matrix_identity_sw(matrix_test_info);
  if(!is_sim())
  {
    matrix_print(matrix_test_info);
    matrix_print(matrix_ref2_info);
  }
  matrix_compare(matrix_test_info,matrix_ref2_info,1);

  //
  printf_section(SKIP_SIM, "subbyte matrix test 3");
  matrix_test_info = matrix_generate_info(MATRIX_DATATYPE_UINT08,MATRIX_SIZE,MATRIX_SIZE,matrix_test,NULL);
  for(int i=0; i<MATRIX_SIZE; i++)
    for(int j=0; j<MATRIX_SIZE; j++)
      matrix_write_fixed_element(matrix_test_info, i, j, i+j+1);
  if(!is_sim())
  {
    matrix_print(matrix_test_info);
    matrix_print(matrix_ref1_info);
  }
  matrix_compare(matrix_test_info,matrix_ref1_info,1);
  
  return 0;
}

