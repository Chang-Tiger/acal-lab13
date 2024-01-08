#ifndef __OP_SCALAR_GEMM_H__
#define __OP_SCALAR_GEMM_H__

#include "acal_lab/includes/op/Op.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "acal_lab/includes/info/op/GemmInfo.h"

namespace acal_lab
{
namespace scalar
{
class Gemm : public Operator
{
  protected:
    gemmInfo *info;
    quantiInfo *qInfo;

  public:
    Gemm(tensorInfo *opt, tensorInfo *ipt, gemmInfo *opInfo, quantiInfo *quantInfo, QauntiType qType)
        : Operator(opt, ipt, qType), info(opInfo), qInfo(quantInfo)
    {
    }

    void execPerLayerNaiveQuant() override;
    void execPerLayerAdvanceQuant() override;
    void execPerOperationNaiveQuant() override;
    void execPerOperationAdvanceQuant() override;
};

} // namespace scalar
} // namespace acal_lab

#endif // __OP_SCALAR_GEMM_H__