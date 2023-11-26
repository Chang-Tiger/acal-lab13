
#ifndef _MY_RELU_H_
#define _MY_RELU_H_

#include "../../includes/myTensor.h"
#include "../../../simd/mySimdInst.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus

extern "C"
{
#endif

    void myReLU(myTensorInfo*, myTensorInfo*);
#ifdef __cplusplus
}
#endif

#endif
