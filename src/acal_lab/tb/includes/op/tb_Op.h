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
    void (tb_Operator::*execFunction)();

  public:
    tb_Operator(tensorInfo *opt, tensorInfo *ipt, QauntiType quantType) : output(opt), input(ipt)
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
        case GENERAL:
            execFunction = &tb_Operator::exec;
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
    virtual void exec() {};
};

bool compare8(tensorInfo *, tensorInfo *);
bool compare16(tensorInfo *, tensorInfo *);
void randomInit8(tensorInfo *);
void randomInit16(tensorInfo *);
} // namespace tb
} // namespace acal_lab

#endif // _TB_OP_OP_H_