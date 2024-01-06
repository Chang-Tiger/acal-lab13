#ifndef _TB_OP_OP_H_
#define _TB_OP_OP_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "acal_lab/includes/info.h"
#include "acal_lab/includes/simd.h"

namespace acal_lab
{
namespace tb
{
typedef enum TestType
{
    LAB,
    HW
} testType;
}; // namespace tb
class tb_Operator
{
  protected:
    tensorInfo *output;
    tensorInfo *input;
    quantiInfo *qInfo;
    void (tb_Operator::*execFunction)();

  public:
    tb_Operator(tensorInfo *opt, tensorInfo *ipt, quantiInfo *quantInfo)
    {
        this->output = opt;
        this->input = ipt;
        this->qInfo = quantInfo;
    }
    void execute()
    {
        (this->*execFunction)();
    };
};

}; // namespace acal_lab

#endif // _TB_OP_OP_H_