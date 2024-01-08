#include "acal_lab/tb/includes/op/simd/tb_Conv.h"

#define TB_SIZE 100

bool acal_lab::tb::tb_ConvPerOperationrNaiveQuant(int8_t *ipt, int8_t *weight, int8_t *bias, int8_t *opt_8,
                                                  int8_t *optTest_8)
{
    int per_opeation_cnt = 0;
    printf("[ TEST ] `CONV`  : per Operation  Naive  Quantization %3d/%3d\n", per_opeation_cnt, TB_SIZE);
    return per_opeation_cnt == TB_SIZE ? true : false;
}

bool acal_lab::tb::tb_ConvPerOperationAdvanceQuant(int8_t *ipt, int8_t *weight, int8_t *bias, int8_t *opt_8,
                                                   int8_t *optTest_8)
{
    int per_opeation_cnt = 0;
    printf("[ TEST ] `CONV`  : per Operation Advance Quantization %3d/%3d\n", per_opeation_cnt, TB_SIZE);
    return per_opeation_cnt == TB_SIZE ? true : false;
}

bool acal_lab::tb::tb_ConvPerLayerNaiveQuant(int8_t *ipt, int8_t *weight, int16_t *bias, int16_t *opt_8,
                                             int16_t *optTest_8)
{
    int per_layer_cnt = 0;
    printf("[ TEST ] `CONV`  : per   Layer    Naive  Quantization %3d/%3d\n", per_layer_cnt, TB_SIZE);
    return per_layer_cnt == TB_SIZE ? true : false;
}

bool acal_lab::tb::tb_ConvPerLayerAdvanceQuant(int8_t *ipt, int8_t *weight, int16_t *bias, int16_t *opt_8,
                                               int16_t *optTest_8)
{
    int per_layer_cnt = 0;
    printf("[ TEST ] `CONV`  : per   Layer   Advance Quantization %3d/%3d\n", per_layer_cnt, TB_SIZE);
    return per_layer_cnt == TB_SIZE ? true : false;
}

bool acal_lab::tb::tb_Conv(testType type)
{
    int8_t ipt[1 * 1] = {0};
    int8_t weight[1 * 1] = {0};
    int8_t bias_8[1 * 1] = {0};
    int16_t bias_16[1 * 1] = {0};
    int8_t opt_8[1 * 1] = {0};
    int8_t optTest_8[1 * 1] = {0};
    int16_t opt_16[1 * 1] = {0};
    int16_t optTest_16[1 * 1] = {0};

    if (type == acal_lab::tb::testType::HW)
        return acal_lab::tb::tb_ConvPerOperationrNaiveQuant(ipt, weight, bias_8, opt_8, optTest_8) &
               acal_lab::tb::tb_ConvPerLayerNaiveQuant(ipt, weight, bias_16, opt_16, optTest_16) &
               acal_lab::tb::tb_ConvPerOperationAdvanceQuant(ipt, weight, bias_8, opt_8, optTest_8) &
               acal_lab::tb::tb_ConvPerLayerAdvanceQuant(ipt, weight, bias_16, opt_16, optTest_16);
    else
        return acal_lab::tb::tb_ConvPerOperationrNaiveQuant(ipt, weight, bias_8, opt_8, optTest_8) &
               acal_lab::tb::tb_ConvPerLayerNaiveQuant(ipt, weight, bias_16, opt_16, optTest_16);
}