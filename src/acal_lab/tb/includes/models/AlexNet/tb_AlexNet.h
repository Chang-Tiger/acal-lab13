#ifndef _TB__MODELS__ALEXNET__TB_ALEXNET_H_
#define _TB__MODELS__ALEXNET__TB_ALEXNET_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "acal_lab/includes/info.h"
#include "acal_lab/includes/models/AlexNet/AlexNet.h"
#include "tb_AlexNetConfig.h"

namespace acal_lab
{
namespace tb
{
typedef enum TestBench
{
    PICTURE_0,
    PICTURE_1
} testbench;

bool tb_AlexNet(testbench);
bool tb_check(tensorInfo output, testbench pic);
} // namespace tb
} // namespace acal_lab

#endif // _TB__MODELS__ALEXNET__TB_ALEXNET_H_