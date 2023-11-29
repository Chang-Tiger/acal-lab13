#ifndef _TB_MODEL_MY_ALEXNET_TB_CFG_H_
#define _TB_MODEL_MY_ALEXNET_TB_CFG_H_

#define dimX 10

// Extra Buffer
#define EXTRA_BUFFER_SIZE 10000

// Input Size
#define DATA_0_C 1
#define DATA_0_H 1
#define DATA_0_W 1

// Layer 1 : Convolution
#define CONV_1_C 1
#define CONV_1_H 1
#define CONV_1_W 1
#define CONV_1_W_C 1
#define CONV_1_W_H 1
#define CONV_1_W_W 1
#define CONV_1_B_C 1
#define CONV_1_B_H 1
#define CONV_1_B_W 1
#define CONV_1_KERNEL_SIZE 1
#define CONV_1_STRIDE 1
#define CONV_1_PADDING 1

// Layer 2 : ReLU
#define RELU_1_C CONV_1_C
#define RELU_1_H CONV_1_H
#define RELU_1_W CONV_1_W
// Layer 3 : MaxPool
#define MXPL_1_C 1
#define MXPL_1_H 1
#define MXPL_1_W 1
#define MXPL_1_KERNEL_SIZE 1
#define MXPL_1_STRIDE 1
#define MXPL_1_PADDING 1
// Layer 4 : Convolution
#define CONV_2_C 1
#define CONV_2_H 1
#define CONV_2_W 1
#define CONV_2_W_C 1
#define CONV_2_W_H 1
#define CONV_2_W_W 1
#define CONV_2_B_C 1
#define CONV_2_B_H 1
#define CONV_2_B_W 1
#define CONV_2_KERNEL_SIZE 1
#define CONV_2_STRIDE 1
#define CONV_2_PADDING 1
// Layer 5 : ReLU
#define RELU_2_C CONV_2_C
#define RELU_2_H CONV_2_H
#define RELU_2_W CONV_2_W
// Layer 6 : MaxPool
#define MXPL_2_C 1
#define MXPL_2_H 1
#define MXPL_2_W 1
#define MXPL_2_KERNEL_SIZE 1
#define MXPL_2_STRIDE 1
#define MXPL_2_PADDING 1
// Layer 7 : Convolution
#define CONV_3_C 1
#define CONV_3_H 1
#define CONV_3_W 1
#define CONV_3_W_C 1
#define CONV_3_W_H 1
#define CONV_3_W_W 1
#define CONV_3_B_C 1
#define CONV_3_B_H 1
#define CONV_3_B_W 1
#define CONV_3_KERNEL_SIZE 1
#define CONV_3_STRIDE 1
#define CONV_3_PADDING 1
// Layer 8 : ReLU
#define RELU_3_C CONV_3_C
#define RELU_3_H CONV_3_H
#define RELU_3_W CONV_3_W
// Layer 9 : Convolution
#define CONV_4_C 1
#define CONV_4_H 1
#define CONV_4_W 1
#define CONV_4_W_C 1
#define CONV_4_W_H 1
#define CONV_4_W_W 1
#define CONV_4_B_C 1
#define CONV_4_B_H 1
#define CONV_4_B_W 1
#define CONV_4_KERNEL_SIZE 1
#define CONV_4_STRIDE 1
#define CONV_4_PADDING 1
// Layer 10 : ReLU
#define RELU_4_C CONV_4_C
#define RELU_4_H CONV_4_H
#define RELU_4_W CONV_4_W
// Layer 11 : Convolution
#define CONV_5_C 1
#define CONV_5_H 1
#define CONV_5_W 1
#define CONV_5_W_C 1
#define CONV_5_W_H 1
#define CONV_5_W_W 1
#define CONV_5_B_C 1
#define CONV_5_B_H 1
#define CONV_5_B_W 1
#define CONV_5_KERNEL_SIZE 1
#define CONV_5_STRIDE 1
#define CONV_5_PADDING 1
// Layer 12 : ReLU
#define RELU_5_C CONV_5_C
#define RELU_5_H CONV_5_H
#define RELU_5_W CONV_5_W
// Layer 13 : MaxPool
#define MXPL_3_C 1
#define MXPL_3_H 1
#define MXPL_3_W 1
#define MXPL_3_KERNEL_SIZE 1
#define MXPL_3_STRIDE 1
#define MXPL_3_PADDING 1
// Layer 14 : GEMM
#define GEMM_1_C 1
#define GEMM_1_H 1
#define GEMM_1_W 1
#define GEMM_1_W_C 1
#define GEMM_1_W_H 1
#define GEMM_1_W_W 1
#define GEMM_1_B_C 1
#define GEMM_1_B_H 1
#define GEMM_1_B_W 1
// Layer 15 : ReLU
#define RELU_6_C GEMM_1_C
#define RELU_6_H GEMM_1_H
#define RELU_6_W GEMM_1_W
// Layer 16 : GEMM
#define GEMM_2_C 1
#define GEMM_2_H 1
#define GEMM_2_W 1
#define GEMM_2_W_C 1
#define GEMM_2_W_H 1
#define GEMM_2_W_W 1
#define GEMM_2_B_C 1
#define GEMM_2_B_H 1
#define GEMM_2_B_W 1
// Layer 17 : ReLU
#define RELU_7_C GEMM_2_C
#define RELU_7_H GEMM_2_H
#define RELU_7_W GEMM_2_W
// Layer 18 : GEMM
#define GEMM_3_C 1
#define GEMM_3_H 1
#define GEMM_3_W 1
#define GEMM_3_W_C 1
#define GEMM_3_W_H 1
#define GEMM_3_W_W 1
#define GEMM_3_B_C 1
#define GEMM_3_B_H 1
#define GEMM_3_B_W 1
#endif
