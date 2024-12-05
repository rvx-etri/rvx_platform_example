#include "platform_info.h"
#include "ervp_printf.h"
#include "ervp_multicore_synch.h"
#include "ervp_mmiox.h"
#include "ervp_assert.h"

// the behavior of the accelerator is written in "XXXX_user_region.vh" file.

uint64_t config_data = 0x01020304050607;
uint32_t config_to_verify[2];

// only "unsigned int" and "int" is allowed when bit field is specified.
typedef struct {
  unsigned int waddr: 32;
  unsigned int wnum: 8;
  unsigned int opcode: 8;
} MyAccInst;

MyAccInst inst[2];

uint32_t acc_core_status;

const uint64_t acc_input_data[3] = {
  1, 3, 5
};

uint64_t acc_output_data[3];

uint32_t acc_core_log;

int main()
{
  inst[0].waddr = 0x30000000;
  inst[0].wnum = 4;
  inst[0].opcode = 7;
  inst[1].waddr = 0xF0000000;
  inst[1].wnum = 8;
  inst[1].opcode = 2;
  mmiox_config_write(I_TEST_MMIOX_BASEADDR, &config_data, sizeof(uint64_t));
  mmiox_config_read(I_TEST_MMIOX_BASEADDR, config_to_verify, sizeof(uint64_t));
  assert((*((uint64_t*)config_to_verify))==config_data);
  mmiox_inst_push(I_TEST_MMIOX_BASEADDR, inst, 2, sizeof(MyAccInst), 0);
  assert(mmiox_inst_num_fifo(I_TEST_MMIOX_BASEADDR)==8);
  assert(mmiox_inst_num_busy(I_TEST_MMIOX_BASEADDR)==0); // due to sim code
  assert(mmiox_core_has_log(I_TEST_MMIOX_BASEADDR)==1); // due to sim code
  mmiox_core_log_read(I_TEST_MMIOX_BASEADDR, &acc_core_log, sizeof(uint32_t));
  assert(acc_core_log==0xdddd); // due to sim code
  mmiox_core_status_read(I_TEST_MMIOX_BASEADDR, &acc_core_status, sizeof(uint32_t));
  assert(acc_core_status==0x010a); // due to sim code
  mmiox_input_push(I_TEST_MMIOX_BASEADDR, acc_input_data, 3, sizeof(uint64_t));
  assert(mmiox_input_num_fifo(I_TEST_MMIOX_BASEADDR)==(5-3));
  mmiox_inst_push(I_TEST_MMIOX_BASEADDR, inst, 2, sizeof(MyAccInst), 0);
  assert(mmiox_inst_num_busy(I_TEST_MMIOX_BASEADDR)==1); // due to sim code
  assert(mmiox_output_num_fifo(I_TEST_MMIOX_BASEADDR)==3); // due to sim code
  //mmiox_output_pop(I_TEST_MMIOX_BASEADDR, acc_output_data, 3, sizeof(uint64_t)); // if num_output is fixed
  int output_index = 0;
  while(1)
  {
    int num_output = mmiox_output_num_fifo(I_TEST_MMIOX_BASEADDR);
    if(num_output==0)
      break;
    mmiox_output_pop(I_TEST_MMIOX_BASEADDR, &(acc_output_data[output_index]), num_output, sizeof(uint64_t));
    output_index += num_output;
  }
  
  for(int i=0; i<3; i++)
    assert(acc_output_data[i]==acc_input_data[i]);
	return 0;
}
