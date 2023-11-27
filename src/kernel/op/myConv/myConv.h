
#ifndef _MY_CONV_H_
#define _MY_CONV_H_

#include "../../../simd/mySimdInst.h"
#include "../../includes/myQuanti.h"
#include "../../includes/myTensor.h"
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
        myTensorInfo extraBuffer;
        myTensorInfo kernel;
        myTensorInfo bias;
    } myConvInfo;
    void myConv(myTensorInfo *, myTensorInfo *, myConvInfo *, myQuantiInfo *);

#ifdef __cplusplus
}
#endif

#endif
