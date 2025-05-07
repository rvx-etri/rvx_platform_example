#include "platform_info.h"
#include "ervp_assert.h"
#include "ervp_printf.h"
#include "ervp_blocked_matrix_op.h"
#include "ervp_matrix_op_transform.h"

#include "ip_instance_info.h"
#include "vta_ctrl_api.h"
#include "vta_matrix_api.h"

#include "map_your_matrix_hw.h"

const char matrix_hw_name[] = "BLOCKED_VTA";

// VTA

static vta_hwinfo_t i_vta0;

void i_vta0_wait()
{
  // vta_ctrl_wait(&i_vta0);
}

ervp_mop_wait_fx_t i_vta0_matrix_mult_16x16le(ervp_mop_mapping_t *mop_mapping, const ErvpMatrixInfo *ma_info, const ErvpMatrixInfo *mb_info, ErvpMatrixInfo *mc_info, int options)
{
  vta_matrix_mult_16x16le(mop_mapping, &i_vta0, ma_info, mb_info, mc_info, options);
  return i_vta0_wait;
}

ervp_mop_wait_fx_t i_vta0_matrix_mult_16x16le_aligned(ervp_mop_mapping_t *mop_mapping, const ErvpMatrixInfo *ma_info, const ErvpMatrixInfo *mb_info, ErvpMatrixInfo *mc_info, int options)
{
  _vta_matrix_mult_16x16le_aligned(mop_mapping, &i_vta0, ma_info, mb_info, mc_info, options);
  return i_vta0_wait;
}

#define BLOCK_SIZE 16

static ervp_blocked_matrix_info_t *blocked_info_for_mult = NULL;

static ervp_mop_wait_fx_t i_vta0_blocked_matrix_mult(ervp_mop_mapping_t *mop_mapping, const ErvpMatrixInfo *a, const ErvpMatrixInfo *b, ErvpMatrixInfo *c, int options)
{
  return blocked_matrix_mult(blocked_info_for_mult, a, b, c, options);
}

void map_your_matrix_function(ervp_mop_mapping_t *mop_mapping)
{
  i_vta0.block_size = BLOCK_SIZE;
  i_vta0.ctrl_addr = I_VTA00_CONFIG_BASEADDR;
  vta_matrix_setup_buffer(&i_vta0);

  /* map your own functions */
  blocked_info_for_mult = blocked_matrix_info_alloc();
  blocked_info_for_mult->block_size = i_vta0.block_size;
  blocked_info_for_mult->subop_mapping = matrix_op_mapping_alloc();
  blocked_info_for_mult->subop_mapping->matrix_mult = i_vta0_matrix_mult_16x16le;

  //
  mop_mapping->matrix_mult = i_vta0_blocked_matrix_mult;
  mop_mapping->matrix_conv = matrix_conv2mult_im2col;
}
