#include <stdint.h>

#include "platform_info.h"
#include "xvta_hw.h"

#include "ervp_printf.h"
#include "ervp_variable_allocation.h"
#include "ervp_mmio_util.h"
#include "ervp_matrix.h"

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
uint32_t insns[28] MUST_DRAM VTA_DATA = {
  0x00000000, 0x00000000, 0x00100001, 0x00000010,  // LOAD
  0x000001a0, 0x00000000, 0x00100001, 0x00000010,  // LOAD
  0x00000090, 0x00000000, 0x00010001, 0x00000001,  // LOAD
  0x00000140, 0x00000000, 0x00100001, 0x00000010,  // LOAD
  0x0200004a, 0x00020008, 0x00000000, 0x00000000,  // GEMM
  0x00000229, 0x00000000, 0x00100001, 0x00000010,  // STORE
  0x00000013, 0x00000000, 0x00000000, 0x00000000}; // FINISH

////// micro operation  (dst_idx, src_idx, wgt_idx)
uint32_t uops[16] MUST_DRAM VTA_DATA = {
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

int vta_status(void)
{
  return REG32(XVTA_CONTROL_ADDR_AP_CTRL);
} 

static uint32_t zero_matrix[1024] BIG_DATA_BSS VTA_DATA;

void matrix_mult_vta(const ErvpMatrixInfo* a, const ErvpMatrixInfo* b, ErvpMatrixInfo* c)
{
  // ap start
  REG32(XVTA_CONTROL_ADDR_AP_CTRL) = 0x0;
  // Data signal of insn_count
  REG32(XVTA_CONTROL_ADDR_INSN_COUNT_DATA) = 7;

  // Data signal of insns
  REG32(XVTA_CONTROL_ADDR_INSNS_DATA)   = insns;
  // Data signal of uops
  REG32(XVTA_CONTROL_ADDR_UOPS_DATA)    = uops;
  // Data signal of inputs
  REG32(XVTA_CONTROL_ADDR_INPUTS_DATA)  = a->addr;
  // Data signal of weights
  REG32(XVTA_CONTROL_ADDR_WEIGHTS_DATA) = b->addr;
  // Data signal of biases
  REG32(XVTA_CONTROL_ADDR_BIASES_DATA)  = zero_matrix;
  // Data signal of outputs
  REG32(XVTA_CONTROL_ADDR_OUTPUTS_DATA) = c->addr;

  REG32(XVTA_CONTROL_ADDR_AP_CTRL) = 0x1;   // ap_start
  printf("\nstart");
  while(1)
  {
    int ap_done = vta_status() & 0x0002;
    if(ap_done)
    {
      printf("\nend");
      break;
    }
  }
}
