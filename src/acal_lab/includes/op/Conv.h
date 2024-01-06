#ifndef _OP_CONV_H_
#define _OP_CONV_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Op.h"
#include "acal_lab/includes/info.h"

namespace acal_lab
{

typedef struct ConvInfo
{
    int kernelSize;
    int stride;
    int padding;
    tensorInfo kernel;
    tensorInfo bias;
    tensorInfo extraBuffer;
} convInfo;

class Conv : public Operator
{
  protected:
    convInfo *info;

  public:
    Conv(tensorInfo *opt, tensorInfo *ipt, convInfo *opInfo, quantiInfo *quantInfo, QauntiType qType)
        : Operator(opt, ipt, quantInfo, qType), info(opInfo)
    {
    }

    void execPerLayerNaiveQuant();
    void execPerLayerAdvanceQuant();
    void execPerOperationNaiveQuant();
    void execPerOperationAdvanceQuant();
};

}; // namespace acal_lab

#endif // _OP_CONV_H_