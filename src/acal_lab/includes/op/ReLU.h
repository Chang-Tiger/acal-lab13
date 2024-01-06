#ifndef _OP_RELU_H_
#define _OP_RELU_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Op.h"

namespace acal_lab
{

class ReLU : public Operator
{
  public:
    ReLU(tensorInfo *opt, tensorInfo *ipt, QauntiType qType) : Operator(opt, ipt, qType)
    {
        execFunction = reinterpret_cast<void (Operator::*)()>(&ReLU::exec);
    }
    void exec();
};

}; // namespace acal_lab

#endif // _OP_RELU_H_