
#ifndef _MY_ALEXNET_H_
#define _MY_ALEXNET_H_

#include "../../kernel/myKernel.h"
#include "../../simd/mySimdInst.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus

extern "C"
{
#endif
    void myAlexNet(myConvInfo *, myGemmInfo *, myMxPlInfo *, myTensorInfo *, myQuantiInfo *);
#ifdef __cplusplus
}
#endif

#endif
