#include "platform_info.h"
#include "ervp_uart.h"
#include "ervp_printf.h"
#include "ervp_multicore_synch.h"
#include "ervp_mmio_util.h"
#include "ervp_external_peri_group_memorymap.h"
#include "ervp_variable_allocation.h"
#include "ervp_memory_util.h"
#include "ervp_core_id.h"

#include "darknet.h"
#include <time.h>
#include <stdio.h>

#include "parser.h"
#include "utils.h"
#include "blas.h"
#include "connected_layer.h"

//#include "img.h"
//#include "image_c6.h"
#include "mnist_seven.h"
#include "mnist_five.h"

int main(int argc, char **argv)
{
  if(EXCLUSIVE_ID==0)
  {
    int top = 0;

    // predict_classifier("mnist_names_list", "mnist_lenet2.cfg",
    //                    "mnist_lenet2_final.weights", mnist_seven, top);
    predict_classifier("mnist_names_list", "mnist_lenet2.cfg",
        "mnist_lenet2_final.weights", mnist_five, top);
  }

  return 0;
}
