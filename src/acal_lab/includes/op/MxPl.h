#ifndef _OP_MXPL_H_
#define _OP_MXPL_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Op.h"

namespace acal_lab
{

typedef struct MaxPoolInfo
{
    int kernelSize;
    int stride;
    int padding;
} mxPlInfo;

class MxPl : public Operator
{
  protected:
    mxPlInfo *info;

  public:
    MxPl(tensorInfo *opt, tensorInfo *ipt, mxPlInfo *opInfo, quantiInfo *quantInfo, QauntiType qType)
        : Operator(opt, ipt, quantInfo, qType), info(opInfo)
    {
        execFunction = reinterpret_cast<void (Operator::*)()>(&MxPl::exec);
    }

    void exec();
};

} // namespace acal_lab

#endif // _OP_MXPL_H_