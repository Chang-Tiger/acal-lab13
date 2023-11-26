
#ifndef _MY_CONV_H_
#define _MY_CONV_H_

#include "../../includes/myTensor.h"
#include "../../../simd/mySimdInst.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus

extern "C"
{
#endif
    typedef struct MyConvInfo
    {
        int kernelSize;
        int stride;
        int padding;
        myTensorInfo kernel;
        myTensorInfo bias;
    } myConvInfo;
    void myConv(myTensorInfo* , myTensorInfo* , myConvInfo* );
#ifdef __cplusplus
}
#endif

#endif
