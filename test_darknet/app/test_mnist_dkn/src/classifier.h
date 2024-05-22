#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include "texpar_list.h"

#ifdef __cplusplus
extern "C" {
#endif
// list *read_data_cfg(char *filename);
void predict_classifier(char *namelistfile, char *cfgfile, char *weightfile, unsigned char *img, int top);
#ifdef __cplusplus
}
#endif
#endif
