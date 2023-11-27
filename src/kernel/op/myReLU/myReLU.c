#include "myReLU.h"

void myReLU(myTensorInfo *outputs, myTensorInfo *inputs, myQuantiInfo *qInfo)
{
    printf("myReLU | \n");
    printf("       | Output Tensor | %d %d %d \n", outputs->data[0], outputs->C, outputs->H, outputs->W);
    printf("       | Input  Tensor | %d %d %d \n", inputs->data[0], inputs->C, inputs->H, inputs->W);
    printf("       | Quantization Information scFactor :%d zeroPt :%d\n", qInfo->scaling_factor, qInfo->zero_point);
}