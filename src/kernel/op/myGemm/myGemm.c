#include "myGemm.h"

void myGemm(myTensorInfo *outputs, myTensorInfo *inputs, myGemmInfo *info, myQuantiInfo *qInfo)
{
    printf("Quantization Information scFactor :%d zeroPt :%d\n", qInfo->scaling_factor, qInfo->zero_point);

    int index_A, index_B, index_C;
    int8_t temp_A[4] = {0}, temp_B[4] = {0};
#ifdef PER_LAYER_QUANTIZATION
    int16_t temp_C[4] = {0};
#else //  PER_OPERATION_QUANTIZATION
    int8_t temp_C[4] = {0};
#endif

    for (int m = 0; m < inputs->H; m++)
    {
        index_A = m * inputs->W;  // M * K
        index_C = m * outputs->W; // M * N
        for (int k = 0; k < inputs->W; k++)
        {
            index_B = k * info->weight.W; // K * N
            for (int n = 0; n < info->weight.W; n += 4)
            {
                *(int32_t *)temp_A = (int32_t)0x01010101 * (int32_t)inputs->data[index_A + k];
                *(int32_t *)temp_B = *(int32_t *)&(info->weight.data)[index_B + n];
                int output_index = index_C + n;
#ifdef PER_LAYER_QUANTIZATION
                sMULI8I16S_vv(temp_C, temp_A, temp_B);
                sADDI16S_vv((int16_t *)&(outputs->data_16[output_index]), (int16_t *)&(outputs->data_16[output_index]),
                            (int16_t *)(temp_C));
                output_index = index_C + 2;
                sADDI16S_vv((int16_t *)&(outputs->data_16[output_index]), (int16_t *)&(outputs->data_16[output_index]),
                            (int16_t *)(temp_C + 2));
#else //  PER_OPERATION_QUANTIZATION
                sMULI8I8S_vv(temp_C, temp_A, temp_B);
                sADDI8S_vv((int8_t *)&(outputs->data[output_index]), (int8_t *)&(outputs->data[output_index]),
                           (int8_t *)(temp_C));
#endif
            }
        }
        for (int n = 0; n < info->weight.W;)
        {
#ifdef PER_LAYER_QUANTIZATION
            sADDI16S_vv((int16_t *)&(outputs->data_16[index_C + n]), (int16_t *)&(outputs->data_16[index_C + n]),
                        (int16_t *)&(info->bias.data_16[index_C + n]));
            n += 2;
#else // PER_OPERATION_QUANTIZATION
            sADDI8S_vv((int8_t *)&(outputs->data[index_C + n]), (int8_t *)&(outputs->data[index_C + n]),
                       (int8_t *)&(info->bias.data[index_C + n]));
            n += 4;
#endif
        }
    }
}

void myGemmScalar(myTensorInfo *outputs, myTensorInfo *inputs, myGemmInfo *info, myQuantiInfo *qInfo)
{
    printf("Quantization Information scFactor :%d zeroPt :%d\n", qInfo->scaling_factor, qInfo->zero_point);
    int8_t temp_A[4] = {0}, temp_B[4] = {0};
    int index_A, index_B, index_C;
    for (int m = 0; m < inputs->H; m++)
    {
        index_A = m * inputs->W;  // M * K
        index_C = m * outputs->W; // M * N
        for (int k = 0; k < inputs->W; k++)
        {
            index_B = k * info->weight.W; // K * N
            for (int n = 0; n < info->weight.W; n += 4)
            {
                *(int32_t *)temp_A = (int32_t)0x01010101 * (int32_t)inputs->data[index_A + k];
                *(int32_t *)temp_B = *(int32_t *)&(info->weight.data)[index_B + n];
                int output_index = index_C + n;
                for (int i = 0; i < 4; i++, output_index++)
#ifdef PER_LAYER_QUANTIZATION
                    outputs->data_16[output_index] += (int16_t)((int32_t)temp_A[i]) * ((int32_t)temp_B[i]);
#else //  PER_OPERATION_QUANTIZATION
                    outputs->data[output_index] += (int8_t)(((int16_t)temp_A[i]) * ((int16_t)temp_B[i]) >> 8);
#endif
            }
        }
        for (int n = 0; n < info->weight.W; n++)
#ifdef PER_LAYER_QUANTIZATION
            outputs->data_16[index_C + n] += info->bias.data_16[index_C + n];
#else // PER_OPERATION_QUANTIZATION
            outputs->data[index_C + n] += info->bias.data[index_C + n];
#endif
    }
}
