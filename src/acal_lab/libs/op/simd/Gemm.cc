#include "acal_lab/includes/op/simd/Gemm.h"

void acal_lab::simd::Gemm::execPerLayerNaiveQuant()
{
    int index_A, index_B, index_C;
    int8_t temp_A[4] = {0}, temp_B[4] = {0};
    int16_t temp_C[4] = {0};

    for (int m = 0; m < input->H; m++)
    {
        index_A = m * input->W; // M * K
        index_C = m * input->W; // M * N
        for (int k = 0; k < input->W; k++)
        {
            index_B = k * info->weight.W; // K * N
            for (int n = 0; n < info->weight.W; n += 4)
            {
                *(int32_t *)temp_A = (int32_t)0x01010101 * (int32_t)input->data[index_A + k];
                *(int32_t *)temp_B = *(int32_t *)&(info->weight.data)[index_B + n];
                int output_index = index_C + n;
                // BUG: use sPMULI8I16S_vv(temp_C, temp_A, temp_B) undefined reference occur
                sPMULI8I16S_vv_L(temp_C, temp_A, temp_B);
                sPMULI8I16S_vv_H(temp_C + 2, temp_A, temp_B);
                sADDI16I16S_vv((int16_t *)&(output->data_16[output_index]), (int16_t *)&(output->data_16[output_index]),
                               (int16_t *)(temp_C));
                output_index = index_C + 2;
                sADDI16I16S_vv((int16_t *)&(output->data_16[output_index]), (int16_t *)&(output->data_16[output_index]),
                               (int16_t *)(temp_C + 2));
            }
        }
        for (int n = 0; n < info->weight.W; n += 2)
            sADDI16I16S_vv((int16_t *)&(output->data_16[index_C + n]), (int16_t *)&(output->data_16[index_C + n]),
                           (int16_t *)&(info->bias.data_16[index_C + n]));
    }
}
void acal_lab::simd::Gemm::execPerOperationNaiveQuant()
{
    int index_A, index_B, index_C;
    int8_t temp_A[4] = {0}, temp_B[4] = {0};

    int8_t temp_C[4] = {0};

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
                sAMULI8I8S_vv_NQ(temp_C, temp_A, temp_B);
                sADDI8I8S_vv((int8_t *)&(output->data[output_index]), (int8_t *)&(output->data[output_index]),
                             (int8_t *)(temp_C));
            }
        }
        for (int n = 0; n < info->weight.W;)
        {
            sADDI8I8S_vv((int8_t *)&(output->data[index_C + n]), (int8_t *)&(output->data[index_C + n]),
                         (int8_t *)&(info->bias.data[index_C + n]));
            n += 4;
        }
    }
}

void acal_lab::simd::Gemm::execPerLayerAdvanceQuant()
{
    sQNT_INFO(qInfo->scaling_factor, qInfo->zero_point);
}

void acal_lab::simd::Gemm::execPerOperationAdvanceQuant()
{
    sQNT_INFO(qInfo->scaling_factor, qInfo->zero_point);
}
