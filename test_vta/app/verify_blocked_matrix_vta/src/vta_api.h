#ifndef __VTA_API_H__
#define __VTA_API_H__

int vta_status(void);
void matrix_mult_vta_16x16(const ErvpMatrixInfo* a, const ErvpMatrixInfo* b, ErvpMatrixInfo* c);

#endif
