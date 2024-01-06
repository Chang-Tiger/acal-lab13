#include "acal_lab/tb/includes/op/tb_Gemm.h"

#define TB_SIZE 100

void acal_lab::tb::TB_Gemm::execPerOperationNaiveQuant()
{
    int8_t temp_A[4] = {0}, temp_B[4] = {0};
    int index_A, index_B, index_C;
    for (int m = 0; m < input->H; m++)
    {
        index_A = m * input->W;  // M * K
        index_C = m * output->W; // M * N
        for (int k = 0; k < input->W; k++)
        {
            index_B = k * info->weight.W; // K * N
            for (int n = 0; n < info->weight.W; n += 4)
            {
                *(int32_t *)temp_A = (int32_t)0x01010101 * (int32_t)input->data[index_A + k];
                *(int32_t *)temp_B = *(int32_t *)&(info->weight.data)[index_B + n];
                int output_index = index_C + n;
                for (int i = 0; i < 4; i++, output_index++)
                {
                    int16_t temp = (int16_t)temp_A[i] * (int16_t)temp_B[i];
                    temp = temp >> 8;
                    output->data[output_index] += temp;
                }
            }
        }
        for (int n = 0; n < info->weight.W; n++)
            output->data[index_C + n] += info->bias.data[index_C + n];
    }
}

void acal_lab::tb::TB_Gemm::execPerOperationAdvanceQuant()
{
}

void acal_lab::tb::TB_Gemm::execPerLayerNaiveQuant()
{
}
void acal_lab::tb::TB_Gemm::execPerLayerAdvanceQuant()
{
}

bool acal_lab::tb::tb_GemmPerOperationrNaiveQuant(int8_t *ipt, int8_t *weight, int8_t *bias, int8_t *gemm_8,
                                                  int8_t *gemmTest_8)
{
    quantiInfo qInfo = {.scaling_factor = 0, .zero_point = 0};
    tensorInfo data_0 = {.C = 1, .H = GEMM_dimM, .W = GEMM_dimK, .data = ipt};
    tensorInfo gemm1_w_0 = {.C = 1, .H = GEMM_dimK, .W = GEMM_dimN, .data = weight};
    tensorInfo gemm1_b_0 = {.C = 1, .H = GEMM_dimM, .W = GEMM_dimN, .data = bias};
    tensorInfo data_1 = {.C = 1, .H = GEMM_dimM, .W = GEMM_dimN, .data = gemm_8};
    tensorInfo data_1_Test = {.C = 1, .H = GEMM_dimM, .W = GEMM_dimN, .data = gemmTest_8};
    gemmInfo gemm1_info = {.weight = gemm1_w_0, .bias = gemm1_b_0};
    int per_opeation_cnt = 0;
    int tb_idx = TB_SIZE;
    while (tb_idx--)
    {
        acal_lab::tb::randomInit8(&data_0);
        acal_lab::tb::randomInit8(&gemm1_w_0);
        acal_lab::tb::randomInit8(&gemm1_b_0);

        acal_lab::Gemm(&data_1, &data_0, &gemm1_info, &qInfo, PER_OPERATION_NAIVE_QUANTI).execute();
        acal_lab::tb::TB_Gemm(&data_1_Test, &data_0, &gemm1_info, &qInfo, PER_OPERATION_NAIVE_QUANTI).execute();

        per_opeation_cnt += acal_lab::tb::compare8(&data_1, &data_1_Test);
    }
    printf("[ TEST ] `GEMM`  : per Operation  Naive  Quantization %3d/%3d\n", per_opeation_cnt, TB_SIZE);
    return per_opeation_cnt == TB_SIZE ? true : false;
}

bool acal_lab::tb::tb_GemmPerLayerNaiveQuant(int8_t *ipt, int8_t *weight, int16_t *bias, int16_t *gemm_16,
                                             int16_t *gemmTest_16)
{
    quantiInfo qInfo = {.scaling_factor = 0, .zero_point = 0};
    tensorInfo data_0 = {.C = 1, .H = GEMM_dimM, .W = GEMM_dimK, .data = ipt};
    tensorInfo gemm1_w_0 = {.C = 1, .H = GEMM_dimK, .W = GEMM_dimN, .data = weight};
    tensorInfo gemm1_b_0 = {.C = 1, .H = GEMM_dimM, .W = GEMM_dimN, .data_16 = bias};
    tensorInfo data_1 = {.C = 1, .H = GEMM_dimM, .W = GEMM_dimN, .data_16 = gemm_16};
    tensorInfo data_1_Test = {.C = 1, .H = GEMM_dimM, .W = GEMM_dimN, .data_16 = gemmTest_16};
    gemmInfo gemm1_info = {.weight = gemm1_w_0, .bias = gemm1_b_0};
    int per_layer_cnt = 0;
    int tb_idx = TB_SIZE;
    while (tb_idx--)
    {
        acal_lab::tb::randomInit8(&data_0);
        acal_lab::tb::randomInit8(&gemm1_w_0);
        acal_lab::tb::randomInit16(&gemm1_b_0);

        acal_lab::Gemm(&data_1, &data_0, &gemm1_info, &qInfo, PER_LAYER_NAIVE_QUANTI).execute();
        acal_lab::tb::TB_Gemm(&data_1_Test, &data_0, &gemm1_info, &qInfo, PER_LAYER_NAIVE_QUANTI).execute();

        per_layer_cnt += acal_lab::tb::compare16(&data_1, &data_1_Test);
    }

    printf("[ TEST ] `GEMM`  : per   Layer    Naive  Quantization %3d/%3d\n", per_layer_cnt, TB_SIZE);
    return per_layer_cnt == TB_SIZE ? true : false;
}

bool acal_lab::tb::tb_GemmPerOperationAdvanceQuant(int8_t *ipt, int8_t *weight, int8_t *bias, int8_t *gemm_8,
                                                   int8_t *gemmTest_8)
{
    int per_opeation_cnt = 0;
    printf("[ TEST ] `GEMM`  : per Operation Advance Quantization %3d/%3d\n", per_opeation_cnt, TB_SIZE);
    return per_opeation_cnt == TB_SIZE ? true : false;
}
bool acal_lab::tb::tb_GemmPerLayerAdvanceQuant(int8_t *ipt, int8_t *weight, int16_t *bias, int16_t *gemm_16,
                                               int16_t *gemmTest_16)
{
    int per_layer_cnt = 0;

    printf("[ TEST ] `GEMM`  : per   Layer   Advance Quantization %3d/%3d\n", per_layer_cnt, TB_SIZE);

    return per_layer_cnt == TB_SIZE ? true : false;
}
bool acal_lab::tb::tb_Gemm(testType type)
{
    int8_t ipt[GEMM_dimM * GEMM_dimK] = {0};
    int8_t weight[GEMM_dimK * GEMM_dimN] = {0};
    int8_t bias_8[GEMM_dimM * GEMM_dimN] = {0};
    int16_t bias_16[GEMM_dimM * GEMM_dimN] = {0};
    int8_t opt_8[GEMM_dimM * GEMM_dimN] = {0};
    int8_t optTest_8[GEMM_dimM * GEMM_dimN] = {0};
    int16_t opt_16[GEMM_dimM * GEMM_dimN] = {0};
    int16_t optTest_16[GEMM_dimM * GEMM_dimN] = {0};

    if (type == acal_lab::tb::testType::HW)
        return acal_lab::tb::tb_GemmPerOperationrNaiveQuant(ipt, weight, bias_8, opt_8, optTest_8) &
               acal_lab::tb::tb_GemmPerLayerNaiveQuant(ipt, weight, bias_16, opt_16, optTest_16) &
               acal_lab::tb::tb_GemmPerOperationAdvanceQuant(ipt, weight, bias_8, opt_8, optTest_8) &
               acal_lab::tb::tb_GemmPerLayerAdvanceQuant(ipt, weight, bias_16, opt_16, optTest_16);
    else
        return acal_lab::tb::tb_GemmPerOperationrNaiveQuant(ipt, weight, bias_8, opt_8, optTest_8) &
               acal_lab::tb::tb_GemmPerLayerNaiveQuant(ipt, weight, bias_16, opt_16, optTest_16);
}