#include "platform_info.h"
#include "ervp_printf.h"
#include "ervp_multicore_synch.h"
#include "ervp_mmiox.h"
#include "ervp_assert.h"

uint64_t value = 0x01020304050607;
uint32_t read_config[2];

// only "unsigned int" and "int" is allowed when bit field is specified.
typedef struct {
  unsigned int waddr: 32;
  unsigned int wnum: 8;
  unsigned int opcode: 8;
} AccInst;

AccInst inst[2];

uint32_t core_status;

const uint64_t original_data[3] = {
  1, 3, 5
};

uint64_t bypass_data[3];

const int USE_INTERRUPT = 1;

int main()
{
  inst[0].waddr = 0x30000000;
  inst[0].wnum = 4;
  inst[0].opcode = 7;
  inst[1].waddr = 0xF0000000;
  inst[1].wnum = 8;
  inst[1].opcode = 2;
  mmiox_config_write(I_TEST_MMIOX_BASEADDR, &value, sizeof(uint64_t));
  mmiox_config_read(I_TEST_MMIOX_BASEADDR, read_config, sizeof(uint64_t));
  assert((*((uint64_t*)read_config))==value);
  mmiox_inst_push(I_TEST_MMIOX_BASEADDR, inst, 2, sizeof(AccInst), USE_INTERRUPT);
  assert(mmiox_inst_num_fifo(I_TEST_MMIOX_BASEADDR)==8);
  assert(mmiox_inst_num_busy(I_TEST_MMIOX_BASEADDR)==1);
  assert(mmiox_inst_has_log(I_TEST_MMIOX_BASEADDR)==0);
  mmiox_core_status_read(I_TEST_MMIOX_BASEADDR, &core_status, sizeof(uint32_t));
  assert(core_status==0x010a);
  if(USE_INTERRUPT)
  {
    assert(mmiox_itr_get_status(I_TEST_MMIOX_BASEADDR)==1);
    mmiox_itr_clear(I_TEST_MMIOX_BASEADDR);
    assert(mmiox_itr_get_status(I_TEST_MMIOX_BASEADDR)==0);
  }
  mmiox_input_push(I_TEST_MMIOX_BASEADDR, original_data, 3, sizeof(uint64_t));
  mmiox_output_pop(I_TEST_MMIOX_BASEADDR, bypass_data, 3, sizeof(uint64_t));
  for(int i=0; i<3; i++)
    assert(bypass_data[i]==original_data[i]);
	return 0;
}
