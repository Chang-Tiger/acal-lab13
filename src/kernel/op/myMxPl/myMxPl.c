#include "myMxPl.h"

void myMxPl(myTensorInfo* outputs, myTensorInfo* inputs, myMxPlInfo* info)
{
    printf("myMxPl | \n");
    printf("       | Output Tensor | %d %d %d \n", outputs->data[0], outputs->C, outputs->H, outputs->W);
    printf("       | Input  Tensor | %d %d %d \n", inputs->data[0], inputs->C, inputs->H, inputs->W);
    printf("       | INFO          | %d %d %d \n", info->kernelSize, info->stride, info->padding);
}