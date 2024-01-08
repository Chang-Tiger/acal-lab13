#ifndef _OP_SIMD_RELU_H_
#define _OP_SIMD_RELU_H_

#include "acal_lab/includes/op/Op.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "acal_lab/includes/info/op/ReLUInfo.h"

namespace acal_lab
{
namespace scalar
{
class ReLU : public Operator
{
  public:
    ReLU(tensorInfo *opt, tensorInfo *ipt, QauntiType qType) : Operator(opt, ipt, qType)
    {
        execFunction = reinterpret_cast<void (Operator::*)()>(&ReLU::exec);
    }
    void exec() override;
};
} // namespace scalar
} // namespace acal_lab

#endif // _OP_SIMD_RELU_H_