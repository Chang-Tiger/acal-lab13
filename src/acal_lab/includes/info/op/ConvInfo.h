#ifndef __INFO_OP_CONV_H__
#define __INFO_OP_CONV_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

namespace acal_lab
{

typedef struct ConvInfo
{
    int kernelSize;
    int stride;
    int padding;
    tensorInfo kernel;
    tensorInfo bias;
} convInfo;
} // namespace acal_lab

#endif // __INFO_OP_CONV_H__