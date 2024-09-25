#include "platform_info.h"
#include "ervp_printf.h"
#include "ervp_core_id.h"
#include "ervp_fakefile.h"
#include "texpar_list.h"
#include "texpar_api.h"

#include <string.h>

#include "npx_struct.h"
#include "npx_parser.h"
#include "npx_sample.h"
#include "npx_tensor.h"
#include "npx_network.h"

char app_name[255] = "cifar10_l4ffff";
char cfg_fname[255];
char parameter_fname[255];
char mnist_fname[255];

int main() {
  if(EXCLUSIVE_ID==0)
  {
    int i = 0;
    int inference_class;
    npx_input_t *npx_input;

    sprintf(cfg_fname, "%s.cfg", app_name);
    npx_network_t *net = npx_parse_network_cfg(cfg_fname);
    printf("The number of layers: %d\n\n", net->num_layers);

    sprintf(parameter_fname, "%s_parameter_quant.bin", app_name);
    npx_load_parameter(net, parameter_fname);

    while(1)
    {
      sprintf(mnist_fname, "%s_sample_value_%03d.bin", app_name, i);
      if(fakefile_dict_find(mnist_fname) == NULL)
        break;

      printf("Image file name: %s\n", mnist_fname);
      npx_input = npx_load_sample(net, mnist_fname);
    
      inference_class = npx_inference(net, npx_input);
      printf("############# Result ############# \n");
      printf("%d / %d (Class / Inference)\n\n\n", npx_input->class, inference_class);
      npx_tensor_free(npx_input->tensor);
      free(npx_input);
      i++;
    }
  
  }

	return 0;
}
