#include <stdint.h>

#include "platform_info.h"

#include "ervp_printf.h"
#include "ervp_variable_allocation.h"
#include "ervp_mmio_util.h"
#include "ervp_matrix_op_sw.h"
#include "ervp_special_matrix_op.h"
#include "ervp_matrix_datatype_define.h"
#include "ervp_assert.h"
#include "ervp_caching.h"

#include "ervp_tensor.h"

#include "vta_hw_spec.h"
#include "vta_inst_api.h"
#include "vta_ctrl_api.h"
#include "vta_matrix_api.h"

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
static uint32_t insns[] NOTCACHED_DATA VTA_DATA = {
  0x00000000, 0x00000000, 0x00100001, 0x00000010,  // LOAD    
  0x000001a0, 0x00000000, 0x00010010, 0x00000088,  // LOAD    
  0x00000090, 0x00000000, 0x00010001, 0x00000040,  // LOAD    
  0x00000140, 0x00000000, 0x00010010, 0x00000088,  // LOAD    
  0x0200004a, 0x00020008, 0x00000000, 0x00000000,  // GEMM    
  0x00000229, 0x00000000, 0x00010010, 0x00000088,  // STORE   
  0x00000013, 0x00000000, 0x00000000, 0x00000000}; // FINISH


////// micro operation  (dst_idx, src_idx, wgt_idx)
static uint32_t uops[] NOTCACHED_DATA VTA_DATA = {
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

#define BUFFER_SIZE (16*16)

static uint32_t left_buffer[BUFFER_SIZE] NOTCACHED_DATA VTA_DATA;
static uint32_t right_buffer[BUFFER_SIZE] NOTCACHED_DATA VTA_DATA;
static const uint32_t zero_buffer[BUFFER_SIZE] BIG_DATA_BSS VTA_DATA;
static uint32_t output_buffer[BUFFER_SIZE] NOTCACHED_DATA VTA_DATA;

static ErvpMatrixInfo left_buffer_info;
static ErvpMatrixInfo right_buffer_info;
static ErvpMatrixInfo output_buffer_info;

static void __attribute__ ((constructor)) construct_vta_block()
{
  matrix_generate_info(MATRIX_DATATYPE_SINT32,16,16,left_buffer,&left_buffer_info);
  matrix_generate_info(MATRIX_DATATYPE_SINT32,16,16,right_buffer,&right_buffer_info);
  matrix_generate_info(MATRIX_DATATYPE_SINT32,16,16,output_buffer,&output_buffer_info);
}

static void update_stride_in_vta_inst(VTAGenericInsn *insn_buf, int input_stride, int weight_stride, int output_stride)
{
  VTAMemInsn *mem_insn = insn_buf;
  // Load Acc
  mem_insn[1].x_stride = output_stride;
  // Load Weight
  mem_insn[2].x_stride = weight_stride;
  // Load Input
  mem_insn[3].x_stride = input_stride;
  // Load Acc
  mem_insn[5].x_stride = output_stride;
}


// DO NOT REMOVE EVEN IF NOT USED
static void generate_vta_inst_for_matrix_mult(const ErvpMatrixInfo *a, const ErvpMatrixInfo *b, ErvpMatrixInfo *c)
{
  assert(a->num_col == b->num_row);
  assert(a->num_row <= 16);
  assert(a->num_col <= 16);
  assert(b->num_col <= 16);

  int batch = 16;
  int in_channels = 16;
  int out_channels = 16;
  int block = 16;
  int virtual_threads = 1;
  int uop_compression = 0;

  int ins_size = batch / block * out_channels / block * (2 + in_channels / block * 3) + 2;
  VTAGenericInsn *insn_buf = getVTABlockedGEMMInsn_block16_stride(matrix_get_stride(a),
      matrix_get_stride(b),
      matrix_get_stride(c) );

  int uop_size = uop_compression ?
    block / VTA_BATCH * virtual_threads :
    block / VTA_BATCH * block / VTA_BLOCK_IN * block / VTA_BLOCK_OUT * virtual_threads;
  VTAUop *uop_buf = getGEMMUops(
      block / VTA_BATCH,
      block / VTA_BLOCK_IN,
      block / VTA_BLOCK_OUT,
      uop_compression,
      virtual_threads > 1);

  printVTADebug(insn_buf, ins_size, uop_buf, uop_size);
}

void vta_matrix_mult_16x16(ervp_mop_mapping_t *mop_mapping, const vta_hwinfo_t *const hwinfo, const ErvpMatrixInfo *a, const ErvpMatrixInfo *b, ErvpMatrixInfo *c, int options)
{
  assert(a->num_col == b->num_row);
  assert(a->num_row <= 16);
  assert(a->num_col <= 16);
  assert(b->num_col <= 16);

  assert(mop_option_has_postprocess(options)==0);

  const int ACC_OUTPUT = mop_option_is_acc(options);
  ervp_mop_wait_fx_t mop_wait_fx = NULL;

  if( (a->num_row == 16) && (a->num_col ==16) && (b->num_col == 16) )
  {
    ErvpMatrixInfo *left_info = &left_buffer_info;
    ErvpMatrixInfo *right_info = &right_buffer_info;
    ErvpMatrixInfo *output_info = &output_buffer_info;

    int batch = 16;
    int in_channels = 16;
    int out_channels = 16;
    int block = 16;
    int virtual_threads = 1;
    int uop_compression = 0;

    int ins_size = batch / block * out_channels / block * (2 + in_channels / block * 3) + 2;
    VTAGenericInsn *insn_buf = (VTAGenericInsn *)insns;

    // Prepare the uop buffer
    int uop_size = uop_compression ?
      block / VTA_BATCH * virtual_threads :
      block / VTA_BATCH * block / VTA_BLOCK_IN * block / VTA_BLOCK_OUT * virtual_threads;
    VTAUop *uop_buf = (VTAUop *)uops;

    //printVTADebug(insn_buf, ins_size, uop_buf, uop_size);

    void *input;
    void *weight;
    void *bias;
    void *output;
    int input_stride = matrix_get_stride(a);
    int weight_stride = matrix_get_stride(right_info);
    int output_stride = matrix_get_stride(c);

    /*** Set a ***/
    int no_copy_a = (((unsigned int)(a->addr) % 8) == 0) &&
                    ((input_stride % 8) == 0) &&
                    (a->datatype == left_info->datatype);
    if(no_copy_a)
    {
      input = a->addr;
    }
    else
    {
      mop_mapping->matrix_copy(mop_mapping, a, left_info, 0);
      input = left_info->addr;
      input_stride = matrix_get_stride(left_info);
    }

    /*** Set b ***/
    weight = right_info->addr;
    mop_wait_fx = mop_mapping->matrix_transpose(mop_mapping, b, right_info, 0);
    matrix_wait_finish(mop_wait_fx);

    /*** Set c ***/
    int no_copy_c = (((unsigned int)(c->addr) % 8) == 0) && 
                    ((output_stride % 8) == 0) && 
                    (c->datatype == output_info->datatype);
    if(no_copy_c) {
      output = c->addr;
    }
    else {
      output = output_info->addr;
      output_stride = matrix_get_stride(output_info);
    }
    if(ACC_OUTPUT) {
      if(no_copy_c) {
        bias = c->addr;
      }
      else {
        mop_wait_fx = mop_mapping->matrix_copy(mop_mapping, c, output_info, 0);
        matrix_wait_finish(mop_wait_fx);
        bias = output_info->addr;
      }
    }
    else {
        bias = zero_buffer;
    }

    update_stride_in_vta_inst(insn_buf, input_stride, weight_stride, output_stride);
    cache_flush_smart(-1);

    vta_ctrl_write(hwinfo, 0x0);
    vta_setup_register(hwinfo, ins_size, insn_buf, uop_buf,input,weight,bias,output);
    // ap_start
    vta_ctrl_write(hwinfo, 0x1);
    vta_ctrl_wait(hwinfo);

    /*** Get c ***/
    if(no_copy_c)
    {
      ;
    }
    else
    {
      mop_wait_fx = mop_mapping->matrix_copy(mop_mapping, output_info, c, 0);
      matrix_wait_finish(mop_wait_fx);
    }
  }
  else
  {
    matrix_mult_sw(a, b, c, options);
  }
}

/*
void vta_matrix_mult_general(ervp_mop_mapping_t *mop_mapping, const vta_hwinfo_t *const hwinfo, const ErvpMatrixInfo *a, const ErvpMatrixInfo *b, ErvpMatrixInfo *c, int options)
{
  ervp_mop_option_t mop_option;
  mop_option.value = options;
  assert((mop_option.br.performs_cliping|mop_option.br.rshift)==0);

  ErvpMatrixInfo block_a = *a;
  ErvpMatrixInfo block_b = *b;
  ErvpMatrixInfo block_c = *c;

  int block_size = hwinfo->block_size;

  ervp_mop_option_t sub_mop_option = mop_option_alloc(0);
  sub_mop_option.br.acc = 1;

  matrix_zero_sw(c);

  for (int m = 0; m < a->num_row; m += block_size)
  {
    int remain_row = a->num_row - m;
    if (remain_row >= block_size)
      remain_row = block_size;
    block_a.num_row = remain_row;
    block_c.num_row = remain_row;

    for (int n = 0; n < b->num_col; n += block_size)
    {
      int remain_col = b->num_col - n;
      if (remain_col >= block_size)
        remain_col = block_size;
      block_b.num_col = remain_col;
      block_c.num_col = remain_col;

      block_c.addr = matrix_get_element_addr(c, m, n);

      for (int k = 0; k < a->num_col; k += block_size)
      {
        int remain_product = a->num_col - k;
        if (remain_product >= block_size)
          remain_product = block_size;
        block_a.num_col = remain_product;
        block_b.num_row = remain_product;

        block_a.addr = matrix_get_element_addr(a, m, k);
        block_b.addr = matrix_get_element_addr(b, k, n);
        
        vta_matrix_mult_16x16(mop_mapping, hwinfo, &block_a, &block_b, &block_c, sub_mop_option.value);
      }
    }
  }
  mop_option_free(sub_mop_option);
}
*/