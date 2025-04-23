#ifndef __VTA_MATRIX_API_H__
#define __VTA_MATRIX_API_H__

#include "ervp_matrix.h"

void vta_matrix_mult_16x16(ervp_mop_mapping_t *mop_mapping, const vta_hwinfo_t *const hwinfo, const ErvpMatrixInfo *a, const ErvpMatrixInfo *b, ErvpMatrixInfo *c, int options);
//void vta_matrix_mult_general(ervp_mop_mapping_t *mop_mapping, const vta_hwinfo_t *const hwinfo, const ErvpMatrixInfo *a, const ErvpMatrixInfo *b, ErvpMatrixInfo *c, int options);

#endif
