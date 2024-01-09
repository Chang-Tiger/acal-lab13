#ifndef __TB_OP_TB_GEMM_H__
#define __TB_OP_TB_GEMM_H__

#include "acal_lab/includes/op/scalar/Gemm.h"
#include "acal_lab/includes/op/simd/Gemm.h"
#include "tb_Op.h"

#define GEMM_dimM 8
#define GEMM_dimN 8
#define GEMM_dimK 8

namespace acal_lab
{
namespace tb
{

bool tb_Gemm(testType);
bool tb_GemmPerOperationrNaiveQuant(int8_t *, int8_t *, int8_t *, int8_t *, int8_t *);
bool tb_GemmPerOperationAdvanceQuant(int8_t *, int8_t *, int8_t *, int8_t *, int8_t *);
bool tb_GemmPerLayerNaiveQuant(int8_t *, int8_t *, int16_t *, int16_t *, int16_t *);
bool tb_GemmPerLayerAdvanceQuant(int8_t *, int8_t *, int16_t *, int16_t *, int16_t *);

} // namespace tb
} // namespace acal_lab

#endif // __TB_OP_TB_GEMM_H__