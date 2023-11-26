#include "myConv.h"

void myConv(myTensorInfo* outputs, myTensorInfo* inputs, myConvInfo* info){


    printf("myConv | \n");
    printf("       | Output Tensor | %d %d %d \n", outputs->data[0], outputs->C, outputs->H, outputs->W);
    printf("       | Input  Tensor | %d %d %d \n", inputs->data[0], inputs->C, inputs->H, inputs->W);
    printf("       | weight Tensor | %d %d %d \n", info->kernel.data[0], info->kernel.C, info->kernel.H, info->kernel.W);
    printf("       | bias   Tensor | %d %d %d \n", info->bias.data[0], info->bias.C, info->bias.H, info->bias.W);
    printf("       | INFO          | %d %d %d \n", info->kernelSize, info->stride, info->padding);

}