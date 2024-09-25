#include "platform_info.h"
#include "ervp_printf.h"
#include "ervp_core_id.h"

// #include "npx_layer.h"
#include "npx_parser.h"

#if 1
const char *cfg_fname = "mnist_app.cfg";
const char *par_fname = "mnist_app_parameter_quant.bin";
const char *inp_fname = "mnist_app_sample_spike_000.bin";
const char *tv_fname  = "mnist_app_testvector_000.bin";
#endif

#if 0
const char *cfg_fname = "fmnist_l2ff.cfg";
const char *par_fname = "fmnist_l2ff_parameter_quant.bin";
const char *inp_fname = "fmnist_l2ff_sample_spike_000.bin";
const char *tv_fname  = "fmnist_l2ff_testvector_000.bin";
#endif

#if 0
const char *cfg_fname = "cifar10_l4ffff.cfg";
const char *par_fname = "cifar10_l4ffff_parameter_quant.bin";
const char *inp_fname = "cifar10_l4ffff_sample_spike_000.bin";
const char *tv_fname  = "cifar10_l4ffff_testvector_000.bin";
#endif

#if 0
const char *cfg_fname = "gtsrb_l4ffff.cfg";
const char *par_fname = "gtsrb_l4ffff_parameter_quant.bin";
const char *inp_fname = "gtsrb_l4ffff_sample_spike_000.bin";
const char *tv_fname  = "gtsrb_l4ffff_testvector_000.bin";
#endif

int main()
{
  if (EXCLUSIVE_ID == 0)
  {
    printf("*** Verify npx : %s ***\n\n", cfg_fname);
    npx_network_t *net = npx_parse_network_cfg(cfg_fname);
    printf("The number of layers: %d\n\n", net->num_layers);
    npx_load_parameter(net, par_fname);
    npx_verify_using_test_vector(net, inp_fname, tv_fname);

    printf("\nspike results");
    npx_tensor_print(net->layers[net->num_layers-1]->output_value_tensor);
  }

  return 0;
}
