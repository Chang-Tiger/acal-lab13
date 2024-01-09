#include "acal_lab/tb/includes/op/simd/tb_Conv.h"

#define TB_SIZE 10

#define CONV_W_N 16
#define CONV_W_C 4
#define CONV_W_H 9
#define CONV_W_W 9
#define CONV_B_W 16
#define CONV_KERNEL_SIZE 9
#define CONV_STRIDE 1
#define CONV_PADDING 0
#define CONV_OPT_C 16
#define CONV_OPT_H 56
#define CONV_OPT_W 56
#define CONV_IPT_C 4
#define CONV_IPT_H 64
#define CONV_IPT_W 64

bool acal_lab::tb::tb_ConvPerOperationrNaiveQuant(tensorInfo *optTest, tensorInfo *opt, tensorInfo *ipt,
                                                  convInfo *opInfo, quantiInfo *quantInfo)
{
    int per_opeation_cnt = 0;
    int tb_idx = TB_SIZE;
    while (tb_idx--)
    {
        acal_lab::tb::randomInit8(ipt);
        acal_lab::tb::randomInit8(&opInfo->bias);
        acal_lab::tb::randomInit8(&opInfo->kernel);
        acal_lab::simd::Conv(opt, ipt, opInfo, quantInfo, PER_OPERATION_NAIVE_QUANTI).execute();
        acal_lab::scalar::Conv(optTest, ipt, opInfo, quantInfo, PER_OPERATION_NAIVE_QUANTI).execute();
        per_opeation_cnt += acal_lab::tb::compare8(optTest, opt);
    }
    printf("[ TEST ] `CONV`  : per Operation  Naive  Quantization %3d/%3d\n", per_opeation_cnt, TB_SIZE);
    return per_opeation_cnt == TB_SIZE ? true : false;
}

bool acal_lab::tb::tb_ConvPerLayerNaiveQuant(tensorInfo *optTest, tensorInfo *opt, tensorInfo *ipt, convInfo *opInfo,
                                             quantiInfo *quantInfo)
{
    int per_layer_cnt = 0;
    int tb_idx = TB_SIZE;
    while (tb_idx--)
    {
        acal_lab::tb::randomInit8(ipt);
        acal_lab::tb::randomInit8(&opInfo->bias);
        acal_lab::tb::randomInit8(&opInfo->kernel);
        acal_lab::simd::Conv(opt, ipt, opInfo, quantInfo, PER_LAYER_NAIVE_QUANTI).execute();
        acal_lab::scalar::Conv(optTest, ipt, opInfo, quantInfo, PER_LAYER_NAIVE_QUANTI).execute();
        per_layer_cnt += acal_lab::tb::compare8(optTest, opt);
    }
    printf("[ TEST ] `CONV`  : per   Layer    Naive  Quantization %3d/%3d\n", per_layer_cnt, TB_SIZE);
    return per_layer_cnt == TB_SIZE ? true : false;
}

bool acal_lab::tb::tb_ConvPerOperationAdvanceQuant(tensorInfo *optTest, tensorInfo *opt, tensorInfo *ipt,
                                                   convInfo *opInfo, quantiInfo *quantInfo)
{
    int per_opeation_cnt = 0;
    int tb_idx = TB_SIZE;
    while (tb_idx--)
    {
        acal_lab::tb::randomInit8(ipt);
        acal_lab::tb::randomInit8(&opInfo->bias);
        acal_lab::tb::randomInit8(&opInfo->kernel);
        // ScalingFactor =  0 ~ 8
        quantInfo->scaling_factor = rand() % 8;
        // ZeroPoint =  0 ~ 127
        quantInfo->zero_point = rand() % 128;
        acal_lab::simd::Conv(opt, ipt, opInfo, quantInfo, PER_OPERATION_ADVANCE_QUANTI).execute();
        acal_lab::scalar::Conv(optTest, ipt, opInfo, quantInfo, PER_OPERATION_ADVANCE_QUANTI).execute();
        per_opeation_cnt += acal_lab::tb::compare8(optTest, opt);
    }
    printf("[ TEST ] `CONV`  : per Operation Advance Quantization %3d/%3d\n", per_opeation_cnt, TB_SIZE);
    return per_opeation_cnt == TB_SIZE ? true : false;
}

bool acal_lab::tb::tb_ConvPerLayerAdvanceQuant(tensorInfo *optTest, tensorInfo *opt, tensorInfo *ipt, convInfo *opInfo,
                                               quantiInfo *quantInfo)
{
    int per_layer_cnt = 0;
    int tb_idx = TB_SIZE;
    while (tb_idx--)
    {
        acal_lab::tb::randomInit8(ipt);
        acal_lab::tb::randomInit8(&opInfo->bias);
        acal_lab::tb::randomInit8(&opInfo->kernel);
        // ScalingFactor =  0 ~ 8
        quantInfo->scaling_factor = rand() % 8;
        // ZeroPoint =  0 ~ 127
        quantInfo->zero_point = rand() % 128;
        acal_lab::simd::Conv(opt, ipt, opInfo, quantInfo, PER_LAYER_ADVANCE_QUANTI).execute();
        acal_lab::scalar::Conv(optTest, ipt, opInfo, quantInfo, PER_LAYER_ADVANCE_QUANTI).execute();
        per_layer_cnt += acal_lab::tb::compare8(optTest, opt);
    }

    printf("[ TEST ] `CONV`  : per   Layer   Advance Quantization %3d/%3d\n", per_layer_cnt, TB_SIZE);
    return per_layer_cnt == TB_SIZE ? true : false;
}

bool acal_lab::tb::tb_Conv(testType type)
{
    int8_t ipt[CONV_IPT_C * CONV_IPT_H * CONV_IPT_W] = {0};
    int8_t wgt[CONV_W_N * CONV_W_C * CONV_W_H * CONV_W_W] = {0};
    int8_t bias[CONV_B_W] = {0};
    int8_t opt[CONV_OPT_C * CONV_OPT_H * CONV_OPT_W] = {0};
    int8_t optTest[CONV_OPT_C * CONV_OPT_H * CONV_OPT_W] = {0};

    tensorInfo data0 = {.N = 1, .C = CONV_IPT_C, .H = CONV_IPT_H, .W = CONV_IPT_W, .data = ipt};
    tensorInfo data1 = {.N = 1, .C = CONV_OPT_C, .H = CONV_OPT_H, .W = CONV_OPT_W, .data = opt};
    tensorInfo data1Test = {.N = 1, .C = CONV_OPT_C, .H = CONV_OPT_H, .W = CONV_OPT_W, .data = optTest};
    tensorInfo convWgt = {.N = CONV_W_N, .C = CONV_W_C, .H = CONV_W_H, .W = CONV_W_W, .data = wgt};
    tensorInfo convBias = {.N = 1, .C = 1, .H = 1, .W = CONV_B_W, .data = bias};
    quantiInfo qInfo = {.scaling_factor = 0, .zero_point = 0};
    convInfo cInfo = {.kernelSize = CONV_KERNEL_SIZE,
                      .stride = CONV_STRIDE,
                      .padding = CONV_PADDING,
                      .kernel = convWgt,
                      .bias = convBias};

    if (type == acal_lab::tb::testType::HW)
        return acal_lab::tb::tb_ConvPerOperationrNaiveQuant(&data1Test, &data1, &data0, &cInfo, &qInfo) &
               acal_lab::tb::tb_ConvPerLayerNaiveQuant(&data1Test, &data1, &data0, &cInfo, &qInfo) &
               acal_lab::tb::tb_ConvPerOperationAdvanceQuant(&data1Test, &data1, &data0, &cInfo, &qInfo) &
               acal_lab::tb::tb_ConvPerLayerAdvanceQuant(&data1Test, &data1, &data0, &cInfo, &qInfo);
    else
        return true;
}