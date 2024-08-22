#include "platform_info.h"
#include "ervp_printf.h"
#include "ervp_core_id.h"

// #include "npx_layer.h"
#include "npx_parser.h"

int main()
{
  if (EXCLUSIVE_ID == 0)
  {
    npx_network_t *net = npx_parse_network_cfg("mnist_app.cfg");
    printf("The number of layers: %d\n\n", net->num_layers);
    npx_load_parameter(net, "mnist_app_parameter_quant.bin");
    npx_verify_using_test_vector(net, "mnist_app_sample_spike_000.bin",
                                      "mnist_app_testvector_000.bin");

    printf("\nspike results");
    npx_tensor_print(net->layers[net->num_layers-1]->output_value_tensor);
  }

  return 0;
}
