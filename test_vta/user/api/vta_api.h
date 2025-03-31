#ifndef __VTA_API_H__
#define __VTA_API_H__

int vta_status(void);

ervp_mop_wait_fx_t matrix_mult_vta_16x16(ervp_mop_mapping_t *mop_mapping, const ErvpMatrixInfo *a, const ErvpMatrixInfo *b, ErvpMatrixInfo *c, int options);
ervp_mop_wait_fx_t matrix_mult_vta(ervp_mop_mapping_t *mop_mapping, const ErvpMatrixInfo *a, const ErvpMatrixInfo *b, ErvpMatrixInfo *c, int options);

#endif
