#include "platform_info.h"
#include "ervp_assert.h"
#include "ervp_printf.h"
#include "ervp_blocked_matrix_op.h"

#include "map_your_matrix_hw.h"

const char matrix_hw_name[] = "BLOCKED_SW";
const int BLOCK_SIZE = 4;

void map_your_matrix_function(ervp_mop_mapping_t* mop_mapping)
{
  /* map your own functions */
  ervp_blocked_matrix_info_t* blocked_info = blocked_matrix_info_alloc();
  blocked_info->block_size = BLOCK_SIZE;
  blocked_info->subop_mapping = matrix_op_mapping_alloc();
  matrix_op_register_blocked_global(mop_mapping, blocked_info);
}
