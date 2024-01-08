#ifndef __INFO_OP_MXPL_H__
#define __INFO_OP_MXPL_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


namespace acal_lab
{

typedef struct MaxPoolInfo
{
    int kernelSize;
    int stride;
    int padding;
} mxPlInfo;

} // namespace acal_lab

#endif // __INFO_OP_MXPL_H__