
#ifndef _MY_RELU_H_
#define _MY_RELU_H_

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

    void myReLU(myTensorInfo *, myTensorInfo *, myQuantiInfo *);
#ifdef __cplusplus
}
#endif

#endif
