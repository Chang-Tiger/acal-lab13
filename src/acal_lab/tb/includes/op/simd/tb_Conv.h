#ifndef __TB_OP_TB_CONV_H__
#define __TB_OP_TB_CONV_H__

#include "acal_lab/includes/op/scalar/Conv.h"
#include "acal_lab/includes/op/simd/Conv.h"
#include "tb_Op.h"

namespace acal_lab
{
namespace tb
{
bool tb_Conv(testType);
bool tb_ConvPerOperationrNaiveQuant(int8_t *, int8_t *, int8_t *, int8_t *, int8_t *);
bool tb_ConvPerOperationAdvanceQuant(int8_t *, int8_t *, int8_t *, int8_t *, int8_t *);
bool tb_ConvPerLayerNaiveQuant(int8_t *, int8_t *, int16_t *, int16_t *, int16_t *);
bool tb_ConvPerLayerAdvanceQuant(int8_t *, int8_t *, int16_t *, int16_t *, int16_t *);
} // namespace tb
} // namespace acal_lab

#endif // __TB_OP_TB_CONV_H__