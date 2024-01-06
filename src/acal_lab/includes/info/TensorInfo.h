#ifndef _INCLUDES_TENSOR_INFO_H
#define _INCLUDES_TENSOR_INFO_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

namespace acal_lab
{
typedef struct TensorInfo
{
    int C;
    int H;
    int W;
    int8_t *data;
    int16_t *data_16;
} tensorInfo;
}; // namespace acal_lab

#endif // _INCLUDES_TENSOR_INFO_H