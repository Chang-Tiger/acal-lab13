
#ifndef _MY_TENSOR_H_
#define _MY_TENSOR_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus

extern "C"
{
#endif
    typedef struct MyTensorInfo
    {
        int8_t *data;
        int16_t *data_16;
        int C;
        int H;
        int W;
    } myTensorInfo;
#ifdef __cplusplus
}
#endif

#endif