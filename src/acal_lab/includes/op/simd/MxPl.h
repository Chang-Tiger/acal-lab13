#ifndef __OP_SIMD_MXPL_H__
#define __OP_SIMD_MXPL_H__

#include "acal_lab/includes/op/Op.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "acal_lab/includes/info/op/MxPlInfo.h"

namespace acal_lab
{
namespace simd
{
class MxPl : public Operator
{
  protected:
    mxPlInfo *info;

  public:
    MxPl(tensorInfo *opt, tensorInfo *ipt, mxPlInfo *opInfo, QauntiType qType) : Operator(opt, ipt, qType), info(opInfo)
    {
        execFunction = reinterpret_cast<void (Operator::*)()>(&MxPl::exec);
    }

    void exec() override;
};
} //  namespace simd
} // namespace acal_lab

#endif // __OP_SIMD_MXPL_H__