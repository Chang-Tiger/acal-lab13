#ifndef _TB_OP_MXPL_H_
#define _TB_OP_MXPL_H_

#include "acal_lab/includes/op/MxPl.h"
#include "tb_Op.h"

namespace acal_lab
{
namespace tb
{
class TB_MxPl : public tb_Operator
{
  protected:
    mxPlInfo *info;

  public:
    TB_MxPl(tensorInfo *opt, tensorInfo *ipt, mxPlInfo *opInfo, QauntiType quantType)
        : tb_Operator(opt, ipt, quantType), info(opInfo)
    {
        execFunction = reinterpret_cast<void (tb_Operator::*)()>(&TB_MxPl::exec);
    }
    void exec();
};

bool tb_MxPl();

} // namespace tb
} // namespace acal_lab
#endif // _TB_OP_MXPL_H_