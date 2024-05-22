#include "platform_info.h"
#include "ervp_memory_util.h"
#include "ervp_profiling.h"
#include "ervp_delay.h"
#include "ervp_variable_allocation.h"
#include "ervp_fakefile.h"

#include "network.h"
#include "utils.h"
#include "parser.h"
#include "blas.h"
#include "assert.h"

#include "classifier.h"
//#include <sys/time.h>


void predict_classifier(char *namelistfile, char *cfgfile, char *weightfile, unsigned char *img, int top)
{
    network net = parse_network_cfg_custom(cfgfile, 1, 0);
    if(weightfile){
        load_weights(&net, weightfile);
    }
    set_batch_network(&net, 1);

    fuse_conv_batchnorm(net);
    calculate_binary_weights(net);

    char *names[100];
    char *line;
    int classes_count = 0;

    FAKEFILE *fp = ffopen(namelistfile, "r");
    while(line = ffgetline(fp))
    {
	    names[classes_count] = line;
	    classes_count++;
    }
    ffclose(fp);

    int classes = classes_count;
    top = 5;

    int i = 0;
    int* indexes = (int*)calloc(top, sizeof(int));
    char buff[256];
    profiling_start("input_init");
    image im = load_image_from_array(img, net.w, net.h, 3);
    // image im = load_image_from_array(img, 28, 28, 3);
    image r = letterbox_image(im, net.w, net.h);
    printf("%d %d\n", r.w, r.h);

    //float *X = r.data;
    float *X;
    unsigned char * quantized_X;
    float *predictions ;
    if (net.quantization_type){
        if(net.start_check_point){
            int input_size = net.layers[net.start_check_point-1].inputs; 
            quantized_X= (unsigned char*)calloc(input_size, sizeof(char));  
           // ffread(quantized_X, sizeof(char), input_size, check_point_output);
        }
        else{
            quantized_X= (unsigned char*)calloc(net.w*net.h*net.c, sizeof(char));  
            printf("init input quantization\n");
            float * mean=(float*)calloc(net.c, sizeof(float)); 
            float * var=(float*)calloc(net.c, sizeof(float)); 
            // mean[0]=net.normalize_mean_0;
            // mean[1]=net.normalize_mean_1;
            // mean[2]=net.normalize_mean_2;
            // var[0]=net.normalize_var_0;
            // var[1]=net.normalize_var_1;
            // var[2]=net.normalize_var_2;
            mean[0]=0.1307;
            mean[1]=0.1307;
            mean[2]=0.1307;
            var[0]=0.3081;
            var[1]=0.3081;
            var[2]=0.3081;
            float input_scale_float=0.0256;
            printf("net.input_scale:%.4f\n",net.input_scale);
            printf("net.input_scale:%x\n",net.input_scale);
            normalize_cpu(im.data, mean, var, 1, 3, net.w*net.h);
            //input quantization can combine with normalize
            for (i=0 ;i<net.h*net.w*net.c; i++)quantized_X[i] = (unsigned char)(round(im.data[i]/input_scale_float)+net.input_zeropoint);
            profiling_end("input_init");    
        }
        //profiling_start("network_predict");
        predictions = quantized_network_predict(net, quantized_X);
        //profiling_start("network_predict");
    }
    else{
        X=im.data;
        //profiling_start("network_predict");
        predictions= network_predict(net, X);  
        //profiling_end("network_predict");      
    }
    //set_check_point("Prediction time");



    //set_check_point(0);
    //print_execution_time();
    printf("\n");

    if(net.hierarchy) hierarchy_predictions(predictions, net.outputs, net.hierarchy, 0);
#if 1
    for(i = 0; i < classes; ++i){
	    if(net.hierarchy) printf("%d, %s: %f, parent: %s \n",i, names[i], predictions[i], (net.hierarchy->parent[i] >= 0) ? names[net.hierarchy->parent[i]] : "Root");
	    else printf("%s: %.06f\n",names[i], predictions[i]);
    }
#else
    top_k(predictions, net.outputs, top, indexes);
    for(i = 0; i < top; ++i){
	    int index = indexes[i];
	    if(net.hierarchy) printf("%d, %s: %f, parent: %s \n",index, names[index], predictions[index], (net.hierarchy->parent[index] >= 0) ? names[net.hierarchy->parent[index]] : "Root");
	    else printf("%s: %f\n",names[index], predictions[index]);
    }
#endif
    if(r.data != im.data) free_image(r);
    free_image(im);
}
