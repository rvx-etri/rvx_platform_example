#include "platform_info.h"
#include "ervp_printf.h"
#include "ervp_core_id.h"
#include "ervp_fakefile.h"
#include "texpar_list.h"
#include "texpar_api.h"

#include <string.h>

#include "npx_struct.h"
#include "npx_parser.h"
#include "npx_mnist.h"
#include "npx_tensor.h"
#include "npx_network.h"

char mnist_fname[10][255] = {
    "mnist_app_sample_value_000.bin",
    "mnist_app_sample_value_001.bin",
    "mnist_app_sample_value_002.bin",
    "mnist_app_sample_value_003.bin",
    "mnist_app_sample_value_004.bin",
    "mnist_app_sample_value_005.bin",
    "mnist_app_sample_value_006.bin",
    "mnist_app_sample_value_007.bin",
    "mnist_app_sample_value_008.bin",
    "mnist_app_sample_value_009.bin"
};

int main() {
  if(EXCLUSIVE_ID==0)
  {
    int i;

    int inference_class;
    npx_input_t *npx_input;
    npx_network_t *net = npx_parse_network_cfg("mnist_app.cfg");
    printf("The number of layers: %d\n\n", net->num_layers);
    npx_load_parameter(net, "mnist_app_parameter_quant.bin");

    for (i = 0; i < 10; i++)
    {
      printf("Image file name: %s\n", mnist_fname[i]);
      npx_input = npx_load_mnist(net, mnist_fname[i]);
      inference_class = npx_inference(net, npx_input);
      printf("############# Result ############# \n");
      printf("%d / %d (Class / Inference)\n\n\n", npx_input->class, inference_class);
      npx_tensor_free(npx_input->tensor);
      free(npx_input);
    }
  
  }

	return 0;
}
