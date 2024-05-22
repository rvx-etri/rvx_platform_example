#include <stdint.h>
#include <stddef.h>
#include "ervp_variable_allocation.h"
#include "ervp_malloc.h"
#include "mnist_names_list_file.h"
#ifndef GENERATE_HEX_USING_GCC
#include "platform_info.h"
#endif

const uint8_t mnist_names_list_file_raw[] ALIGNED_DATA BIG_DATA = {
0x30,0xa,0x31,0xa,0x32,0xa,0x33,0xa,0x34,0xa,0x35,0xa,0x36,0xa,0x37,0xa,0x38,0xa,0x39
};

static char mnist_names_list_file_filename[] = "mnist_names_list";
static fakefile_chunk_t mnist_names_list_file_fakefile_chunk;
static FAKEFILE mnist_names_list_file;

#ifdef USE_FAKEFILE
static void __attribute__ ((constructor)) construct_mnist_names_list_file()
{
	fakefile_chunk_init_(&mnist_names_list_file_fakefile_chunk);
	mnist_names_list_file_fakefile_chunk.data = (uint8_t*)mnist_names_list_file_raw;
	mnist_names_list_file_fakefile_chunk.size = sizeof(mnist_names_list_file_raw);
	mnist_names_list_file_fakefile_chunk.current_size = sizeof(mnist_names_list_file_raw);
	fakefile_init_(&mnist_names_list_file);
	mnist_names_list_file.status = FILE_STATUS_READ_ONLY;
	mnist_names_list_file.name = mnist_names_list_file_filename;
	mnist_names_list_file.head = &mnist_names_list_file_fakefile_chunk;
	fakefile_dict_add(&mnist_names_list_file);
};
static void __attribute__ ((destructor)) destruct_mnist_names_list_file()
{
};
#endif