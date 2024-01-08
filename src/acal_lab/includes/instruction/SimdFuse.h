#ifndef _SIMD_FUSE_H_
#define _SIMD_FUSE_H_
#include <stdint.h>
#include <stdlib.h>

#include "SimdInst.h"

namespace acal_lab
{

// 8-2.1-3.3 : Signed Integer Multiplication : sPMULI8I16S_vv
void sPMULI8I16S_vv(int16_t c[4], int8_t a[4], int8_t b[4]);

// 8-2.1-3.6 : Signed Integer Precise(Lossless) Multiplication : sPMULI8I16S_vx
void sPMULI8I16S_vx(int16_t c[4], int8_t a[4], int8_t b);

} // namespace acal_lab

#endif //_SIMD_FUSE_H_