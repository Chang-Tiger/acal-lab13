#ifndef _OP_GEMM_H_
#define _OP_GEMM_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Op.h"
#include "acal_lab/includes/info.h"
#include "acal_lab/includes/simd.h"

namespace acal_lab
{

typedef struct GemmInfo
{
    tensorInfo weight;
    tensorInfo bias;
} gemmInfo;

class Gemm : public Operator
{
  protected:
    gemmInfo *info;

  public:
    Gemm(tensorInfo *opt, tensorInfo *ipt, gemmInfo *opInfo, quantiInfo *quantInfo, QauntiType qType)
        : Operator(opt, ipt, quantInfo, qType), info(opInfo)
    {
    }

    void execPerLayerNaiveQuant();
    void execPerLayerAdvanceQuant();
    void execPerOperationNaiveQuant();
    void execPerOperationAdvanceQuant();
};

}; // namespace acal_lab

#endif // _OP_GEMM_H_