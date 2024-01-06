#ifndef _TB_OP_RELU_H_
#define _TB_OP_RELU_H_

#include "acal_lab/includes/op/ReLU.h"
#include "tb_Op.h"

namespace acal_lab
{
namespace tb
{
class TB_ReLU : public tb_Operator
{
  public:
    TB_ReLU(tensorInfo *opt, tensorInfo *ipt, QauntiType quantType) : tb_Operator(opt, ipt, quantType)
    {
        execFunction = reinterpret_cast<void (tb_Operator::*)()>(&TB_ReLU::exec);
    }
    void exec();
};

bool tb_ReLU();

} // namespace tb
} // namespace acal_lab
#endif // _TB_OP_RELU_H_