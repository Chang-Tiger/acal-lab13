#ifndef _TB_OP_OP_H_
#define _TB_OP_OP_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "acal_lab/includes/info.h"
#include "acal_lab/includes/simd.h"

#define _8_BIT_ 256
#define _16_BIT_ 65536

namespace acal_lab
{
namespace tb
{
typedef enum TestType
{
    LAB,
    HW
} testType;

class tb_Operator
{
  protected:
    tensorInfo *output;
    tensorInfo *input;
    quantiInfo *qInfo;
    void (tb_Operator::*execFunction)();

  public:
    tb_Operator(tensorInfo *opt, tensorInfo *ipt, quantiInfo *quantInfo, QauntiType quantType)
        : output(opt), input(ipt), qInfo(quantInfo)
    {
        switch (quantType)
        {
        case PER_LAYER_NAIVE_QUANTI:
            execFunction = &tb_Operator::execPerLayerNaiveQuant;
            break;
        case PER_LAYER_ADVANCE_QUANTI:
            execFunction = &tb_Operator::execPerLayerAdvanceQuant;
            break;
        case PER_OPERATION_NAIVE_QUANTI:
            execFunction = &tb_Operator::execPerOperationNaiveQuant;
            break;
        case PER_OPERATION_ADVANCE_QUANTI:
            execFunction = &tb_Operator::execPerOperationAdvanceQuant;
            break;
        }
    }
    void execute()
    {
        (this->*execFunction)();
    };
    virtual void execPerLayerNaiveQuant() = 0;
    virtual void execPerLayerAdvanceQuant() = 0;
    virtual void execPerOperationNaiveQuant() = 0;
    virtual void execPerOperationAdvanceQuant() = 0;
};

bool compare8(tensorInfo *, tensorInfo *);
bool compare16(tensorInfo *, tensorInfo *);
void randomInit8(tensorInfo *);
void randomInit16(tensorInfo *);
}; // namespace tb
}; // namespace acal_lab

#endif // _TB_OP_OP_H_