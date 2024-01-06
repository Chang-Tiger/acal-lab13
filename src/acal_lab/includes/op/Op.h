#ifndef _OP_OP_H_
#define _OP_OP_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "acal_lab/includes/info.h"
#include "acal_lab/includes/simd.h"

namespace acal_lab
{
class Operator
{
  protected:
    tensorInfo *output;
    tensorInfo *input;
    quantiInfo *qInfo;
    void (Operator::*execFunction)();

  public:
    Operator(tensorInfo *opt, tensorInfo *ipt, quantiInfo *quantInfo, qauntiType qType)
        : output(opt), input(ipt), qInfo(quantInfo)
    {
        switch (qType)
        {
        case PER_LAYER_NAIVE_QUANTI:
            execFunction = &Operator::execPerLayerNaiveQuant;
            break;
        case PER_LAYER_ADVANCE_QUANTI:
            execFunction = &Operator::execPerLayerAdvanceQuant;
            break;
        case PER_OPERATION_NAIVE_QUANTI:
            execFunction = &Operator::execPerOperationNaiveQuant;
            break;
        case PER_OPERATION_ADVANCE_QUANTI:
            execFunction = &Operator::execPerOperationAdvanceQuant;
            break;
        }
    }
    void execute()
    {
        (this->*execFunction)();
    };
    virtual void execPerLayerNaiveQuant() {};
    virtual void execPerLayerAdvanceQuant() {};
    virtual void execPerOperationNaiveQuant() {};
    virtual void execPerOperationAdvanceQuant() {};
};

}; // namespace acal_lab

#endif // _OP_GEMM_H_