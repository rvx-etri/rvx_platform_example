#include <stdint.h>

#include "platform_info.h"
#include "xvta_hw.h"

#include "ervp_printf.h"
#include "ervp_variable_allocation.h"
#include "ervp_mmio_util.h"
#include "ervp_matrix.h"
#include "ervp_matrix_op_sw.h"
#include "ervp_special_matrix_op.h"
#include "ervp_matrix_datatype_define.h"
#include "ervp_assert.h"

#include "ervp_tensor.h"
#include "core_dependent.h"

#define VTA_DATA __attribute__ ((aligned(0x1000)))

// instruction 128 bit
typedef struct uint128_s
{
  uint64_t lsb;
  uint64_t msb;
} uint128_t;

// Register Map
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//       *bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read/COR)
//        bit 7  - auto_restart (Read/Write)
//        bit 9  - interrupt (Read)
//        others - reserved

//=====================================================================================
//INFO - Blocked GEMM test: batch=16, in_channels=16, out_channels=16, uop_comp=0
//inp_size=16 wgt_size=1 out_size=16
//batch =16 [16][16] : max 
uint32_t insns[] NOTCACHED_DATA VTA_DATA = {
  0x00000000, 0x00000000, 0x00100001, 0x00000010,  // LOAD
  0x000001a0, 0x00000000, 0x00100001, 0x00000010,  // LOAD
  0x00000090, 0x00000000, 0x00010001, 0x00000001,  // LOAD
  0x00000140, 0x00000000, 0x00100001, 0x00000010,  // LOAD
  0x0200004a, 0x00020008, 0x00000000, 0x00000000,  // GEMM
  0x00000229, 0x00000000, 0x00100001, 0x00000010,  // STORE
  0x00000013, 0x00000000, 0x00000000, 0x00000000}; // FINISH

////// micro operation  (dst_idx, src_idx, wgt_idx)
uint32_t uops[] NOTCACHED_DATA VTA_DATA = {
  0x00000000,
  0x00000801,
  0x00001002,
  0x00001803,
  0x00002004,
  0x00002805,
  0x00003006,
  0x00003807,
  0x00004008,
  0x00004809,
  0x0000500a,
  0x0000580b,
  0x0000600c,
  0x0000680d,
  0x0000700e,
  0x0000780f};

static int vta_status(void)
{
  return REG32(XVTA_CONTROL_ADDR_AP_CTRL);
}

#define BUFFER_SIZE (16*16)

static uint32_t left_buffer[BUFFER_SIZE] NOTCACHED_DATA VTA_DATA;
static uint32_t right_buffer[BUFFER_SIZE] NOTCACHED_DATA VTA_DATA;
static uint32_t zero_buffer[BUFFER_SIZE] NOTCACHED_DATA VTA_DATA;
static uint32_t output_buffer[BUFFER_SIZE] NOTCACHED_DATA VTA_DATA;

static ErvpMatrixInfo left_buffer_info;
static ErvpMatrixInfo right_buffer_info;
static ErvpMatrixInfo output_buffer_info;

static int printing_num_vta = 1;
static int num_vta = 0;

static inline void update_printing_num_vta()
{
  if(printing_num_vta>=10000)
    printing_num_vta += 10000;
  else
    printing_num_vta *= 10;
}

static void setup_vta_var()
{
  // Data signal of insn_count
  REG32(XVTA_CONTROL_ADDR_INSN_COUNT_DATA) = sizeof(insns)/4/4;
  // Data signal of insns
  REG32(XVTA_CONTROL_ADDR_INSNS_DATA)   = insns;
  // Data signal of uops
  REG32(XVTA_CONTROL_ADDR_UOPS_DATA)    = uops;
  // Data signal of inputs
  REG32(XVTA_CONTROL_ADDR_INPUTS_DATA)  = left_buffer_info.addr;
  // Data signal of weights
  REG32(XVTA_CONTROL_ADDR_WEIGHTS_DATA) = right_buffer_info.addr;
  // Data signal of biases
  REG32(XVTA_CONTROL_ADDR_BIASES_DATA)  = zero_buffer;
  // Data signal of outputs
  REG32(XVTA_CONTROL_ADDR_OUTPUTS_DATA) = output_buffer_info.addr;
}

static void __attribute__ ((constructor)) construct_vta()
{
  matrix_generate_info(MATRIX_DATATYPE_SINT32,16,16,left_buffer,&left_buffer_info);
  matrix_generate_info(MATRIX_DATATYPE_SINT32,16,16,right_buffer,&right_buffer_info);
  matrix_generate_info(MATRIX_DATATYPE_SINT32,16,16,output_buffer,&output_buffer_info);
  setup_vta_var();
}

ervp_mop_wait_fx_t matrix_mult_vta_16x16(ervp_mop_mapping_t *mop_mapping, const ErvpMatrixInfo *a, const ErvpMatrixInfo *b, ErvpMatrixInfo *c, int options)
{
  assert(a->num_row == 16);
  assert(a->num_col == 16);
  assert(b->num_row == 16);
  assert(b->num_col == 16);
  /*
  assert(c->num_row == 16);
  assert(c->num_col == 16);
  assert(matrix_datatype_get_num_bits(a->datatype)==32);
  assert(matrix_datatype_get_num_bits(b->datatype)==32);
  assert(matrix_datatype_get_num_bits(c->datatype)==32);
  */

  ErvpMatrixInfo *left_info = &left_buffer_info;
  ErvpMatrixInfo *right_info = &right_buffer_info;
  ErvpMatrixInfo *output_info = &output_buffer_info;

  matrix_copy_sw(a, left_info, 0);
  matrix_transpose_sw(b, right_info, 0);
#ifdef CACHING_ALL
  flush_cache();
#endif

  // ap start
  REG32(XVTA_CONTROL_ADDR_AP_CTRL) = 0x0;
  REG32(XVTA_CONTROL_ADDR_AP_CTRL) = 0x1;   // ap_start

#ifdef CHECK_VTA
  ++num_vta;
  if(num_vta==printing_num_vta)
    printf("\nvta start %d", num_vta);
#endif
  while(1)
  {
    int ap_done = vta_status() & 0x0002;
    if(ap_done)
    {
#ifdef CHECK_VTA
    if(num_vta==printing_num_vta)
    {
      printf("\nvta end %d", num_vta);
      update_printing_num_vta();
    }
#endif
      break;
    }
  }

  matrix_copy_sw(output_info, c, options);
  flush_cache();
  return NULL;
}

ervp_mop_wait_fx_t matrix_mult_vta(ervp_mop_mapping_t *mop_mapping, const ErvpMatrixInfo *a, const ErvpMatrixInfo *b, ErvpMatrixInfo *c, int options)
{
  assert(a->num_row<= 16);
  assert(a->num_col<= 16);
  assert(b->num_row<= 16);
  assert(b->num_col<= 16);

  ErvpMatrixInfo *left_info = &left_buffer_info;
  ErvpMatrixInfo *right_info = &right_buffer_info;
  ErvpMatrixInfo *output_info = &output_buffer_info;
  
  matrix_zero_sw(left_info);
  matrix_copy_part_sw(a, left_info, a->num_row, a->num_col, 0);
  matrix_zero_sw(right_info);
  matrix_transpose_part_sw(b, right_info, b->num_row, b->num_col, 0);
#ifdef CACHING_ALL
  flush_cache();
#endif

  // ap start
  REG32(XVTA_CONTROL_ADDR_AP_CTRL) = 0x0;
  REG32(XVTA_CONTROL_ADDR_AP_CTRL) = 0x1;   // ap_start
#ifdef CHECK_VTA
  ++num_vta;
  if(num_vta==printing_num_vta)
    printf("\nvta start %d", num_vta);
#endif
  while(1)
  {
    int ap_done = vta_status() & 0x0002;
    if(ap_done)
    {
#ifdef CHECK_VTA
    if(num_vta==printing_num_vta)
    {
      printf("\nvta end %d", num_vta);
      update_printing_num_vta();
    }
#endif
      break;
    }
  }

  matrix_copy_part_sw(output_info, c, c->num_row, c->num_col, options);
  return NULL;
}
