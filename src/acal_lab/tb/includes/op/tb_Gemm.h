#ifndef _TB_OP_GEMM_H_
#define _TB_OP_GEMM_H_

#include "acal_lab/includes/op/Gemm.h"
#include "tb_Op.h"

#define GEMM_dimM 8
#define GEMM_dimN 8
#define GEMM_dimK 8

namespace acal_lab
{
namespace tb
{
class TB_Gemm : public tb_Operator
{
  protected:
    gemmInfo *info;
    quantiInfo *qInfo;

  public:
    TB_Gemm(tensorInfo *opt, tensorInfo *ipt, gemmInfo *opInfo, quantiInfo *quantInfo, QauntiType quantType)
        : tb_Operator(opt, ipt, quantType), info(opInfo), qInfo(quantInfo)
    {
    }
    void execPerLayerNaiveQuant();
    void execPerLayerAdvanceQuant();
    void execPerOperationNaiveQuant();
    void execPerOperationAdvanceQuant();
};

bool tb_Gemm(testType);
bool tb_GemmPerOperationrNaiveQuant(int8_t *, int8_t *, int8_t *, int8_t *, int8_t *);
bool tb_GemmPerOperationAdvanceQuant(int8_t *, int8_t *, int8_t *, int8_t *, int8_t *);
bool tb_GemmPerLayerNaiveQuant(int8_t *, int8_t *, int16_t *, int16_t *, int16_t *);
bool tb_GemmPerLayerAdvanceQuant(int8_t *, int8_t *, int16_t *, int16_t *, int16_t *);

} // namespace tb
} // namespace acal_lab

#endif // _TB_OP_GEMM_H_