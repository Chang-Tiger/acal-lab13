
#ifndef _MY_MXPL_H_
#define _MY_MXPL_H_

#include "../../includes/myTensor.h"
#include "../../../simd/mySimdInst.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus

extern "C"
{
#endif

    typedef struct MyMaxPoolInfo
    {
        int kernelSize;
        int stride;
        int padding;
    } myMxPlInfo;

    void myMxPl(myTensorInfo*, myTensorInfo*, myMxPlInfo*);
#ifdef __cplusplus
}
#endif

#endif
