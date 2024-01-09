#ifndef __INFO_OP_GEMM_H__
#define __INFO_OP_GEMM_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

namespace acal_lab
{

typedef struct GemmInfo
{
    tensorInfo weight;
    tensorInfo bias;
} gemmInfo;

} // namespace acal_lab

#endif // __INFO_OP_GEMM_H__