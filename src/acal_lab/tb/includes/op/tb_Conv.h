#ifndef _TB_OP_CONV_H_
#define _TB_OP_CONV_H_

#include "acal_lab/includes/op/Conv.h"
#include "tb_Op.h"

namespace acal_lab
{

namespace tb
{

class TB_Conv : public tb_Operator
{
  protected:
    convInfo *info;

  public:
    TB_Conv(tensorInfo *opt, tensorInfo *ipt, convInfo *opInfo, quantiInfo *quantInfo, QauntiType quantType)
        : tb_Operator(opt, ipt, quantInfo, quantType), info(opInfo)
    {
    }
    void execPerLayerNaiveQuant();
    void execPerLayerAdvanceQuant();
    void execPerOperationNaiveQuant();
    void execPerOperationAdvanceQuant();
};

bool tb_Conv(testType);

bool tb_ConvPerOperationrNaiveQuant(int8_t *, int8_t *, int8_t *, int8_t *, int8_t *);
bool tb_ConvPerOperationAdvanceQuant(int8_t *, int8_t *, int8_t *, int8_t *, int8_t *);
bool tb_ConvPerLayerNaiveQuant(int8_t *, int8_t *, int16_t *, int16_t *, int16_t *);
bool tb_ConvPerLayerAdvanceQuant(int8_t *, int8_t *, int16_t *, int16_t *, int16_t *);

}; // namespace tb

}; // namespace acal_lab

#endif // _TB_OP_CONV_H_