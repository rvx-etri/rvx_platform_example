#include "platform_info.h"
#include "ervp_assert.h"
#include "ervp_printf.h"
#include "ervp_blocked_matrix_op.h"

#include "vta_ctrl_api.h"
#include "vta_matrix_api.h"

#include "map_your_matrix_hw.h"

const char matrix_hw_name[] = "BLOCKED_VTA";

static vta_hwinfo_t i_vta0;

ervp_mop_wait_fx_t i_vta0_matrix_mult_16x16(ervp_mop_mapping_t *mop_mapping, const ErvpMatrixInfo *ma_info, const ErvpMatrixInfo *mb_info, ErvpMatrixInfo *mc_info, int options)
{
  vta_matrix_mult_16x16(mop_mapping, &i_vta0, ma_info, mb_info, mc_info, options);
  return NULL;
}

static ervp_mop_mapping_t subop_mapping;
static ervp_blocked_matrix_info_t blocked_info;

#define BLOCK_SIZE 16

void map_your_matrix_function(ervp_mop_mapping_t* mop_mapping)
{
  i_vta0.block_size = BLOCK_SIZE;
  i_vta0.ctrl_addr = I_VTA00_CONFIG_BASEADDR;

  /* map your own functions */
  ervp_blocked_matrix_info_t* blocked_info = blocked_matrix_info_alloc();
  blocked_info->block_size = i_vta0.block_size;
  blocked_info->subop_mapping = matrix_op_mapping_alloc();
  blocked_info->subop_mapping->matrix_mult = i_vta0_matrix_mult_16x16;

  matrix_op_register_blocked_global(mop_mapping, blocked_info);
}
