#include "acal_lab/includes/op/simd/Conv.h"
#include <stdio.h>
#include <stdlib.h>
namespace acal_lab {
namespace simd {
inline void sPMULI8I16S_vv(int16_t c[4], int8_t a[4], int8_t b[4]) {
	sPMULI8I16S_vv_L(c, a, b);
	sPMULI8I16S_vv_H(c + 2, a, b);
}
void Conv::execPerLayerNaiveQuant() {
	/********************************************************************
	 * TODO:                                                            *
	 * For Homework 13.4, your task is to implement CONV with per Layer *
	 * `Naive Quantization`. This involves using the instruction        *
	 * `sPMULI8I16S(.vv/.vx)` to generate int16 output. However, the    *
	 * int16 output needs to be converted to int8 and then forwarded    *
	 * to the next Operator. To achieve this, utilize `sQNTI16I8S.vv.NQ`*
	 * for the conversion.                                              *
	 *******************************************************************/
}

void Conv::execPerLayerAdvanceQuant() {
	/***********************************************************
	 * TODO:                                                   *
	 * For Homework 13.4, implement CONV with per Operation    *
	 * `Advance Quantization`. Update instruction in the       *
	 * `void acal_lab::Conv::execPerLayerNaiveQuant()`         *
	 * function from `sQNTI16I8S.vv.NQ` to `sQNTI16I8S.vv.AQ`. *
	 **********************************************************/
	int8_t  temp_A[4], temp_B[4];
	int16_t temp_C[4], temp_D[4]; 
	int output_idx, res_idx, k_W_round;
	k_W_round = (info->kernel.W >> 2) << 2; 
	sQNT_INFO(qInfo->scaling_factor, qInfo->zero_point);

	for (int n = 0; n < info->kernel.N; n++) {
		for (int oh = 0; oh < output->H; oh++) {
			for (int ow = 0; ow < output->W; ow++) {

				output_idx = n * output->H * output->W + oh * output->W + ow;
				res_idx = output_idx % 4;
				temp_D[res_idx] = info->bias.data[n];

				for (int c = 0; c < info->kernel.C; c++) {
					for (int kh = 0; kh < info->kernel.H; kh++) {

						int input_idx = c * input->H * input->W + (oh + kh) * input->W + ow;
						int kernel_idx = ((n * info->kernel.C + c) * info->kernel.H + kh) * info->kernel.W;
						
						// Divide Loop by 4
						for (int kw = 0; kw < k_W_round; kw += 4) {
							for (int i=0; i<4;++i){
								temp_A[i] =  input->data[input_idx + kw+i];
								temp_B[i] = info->kernel.data[kernel_idx + kw + i];
							}
							// temp_A[0] = input->data[input_idx + kw];
							// temp_A[1] = input->data[input_idx + kw + 1];
							// temp_A[2] = input->data[input_idx + kw + 2];
							// temp_A[3] = input->data[input_idx + kw + 3];

							// temp_B[0] = info->kernel.data[kernel_idx + kw];
							// temp_B[1] = info->kernel.data[kernel_idx + kw + 1];
							// temp_B[2] = info->kernel.data[kernel_idx + kw + 2];
							// temp_B[3] = info->kernel.data[kernel_idx + kw + 3];

							// SIMD MUL
							sPMULI8I16S_vv(temp_C, temp_A, temp_B);
							
							// Sum Result
							for (int i=0; i<4;++i){
								temp_D[res_idx] += temp_C[i];
							}
							// temp_D[res_idx] += temp_C[0];
							// temp_D[res_idx] += temp_C[1];
							// temp_D[res_idx] += temp_C[2];
							// temp_D[res_idx] += temp_C[3];
						}
						for (int kw = k_W_round; kw < info->kernel.W; kw++) {
							temp_D[res_idx] += (int16_t)input->data[input_idx + kw] * (int16_t)info->kernel.data[kernel_idx + kw];
						}
					}
				}
				// PER LAYER QUANTIZATION
				//do Quantization when accumulate 4 output
				if (res_idx == 3) {
					sQNTI16I8S_vv_AQ(temp_A, temp_D, temp_D + 2);
					output->data[output_idx - 3] = temp_A[0];
					output->data[output_idx - 2] = temp_A[1];
					output->data[output_idx - 1] = temp_A[2];
					output->data[output_idx]     = temp_A[3];
				}
			}
		}
	}
	// PER LAYER QUANTIZATION for remain
	if (res_idx != 3) {
		for (int i = 0; i <= res_idx; i++) {
			output->data[output_idx - (res_idx - i)] = (int8_t)((temp_D[i] >> qInfo->scaling_factor) + qInfo->zero_point);
		}
	}
}

void Conv::execPerOperationNaiveQuant() {
	/********************************************************
	 * TODO:                                                *
	 * For Homework 13.3, implement CONV with per Operation *
	 * Naive Quantization. Utilize `sAMULI8I8S(.vv/.vx).NQ` *
	 * to generate int8 output.                             *
	 *******************************************************/
}

void Conv::execPerOperationAdvanceQuant() {
	/********************************************************
	 * TODO:                                                *
	 * For Homework 8.4, implement CONV with per Operation  *
	 * Advance Quantization. Update instruction in the      *
	 * `void acal_lab::Conv::execPerOperationNaiveQuant()`  *
	 * function from `sAMULI8I8S(.vv/.vx).NQ` to            *
	 * `sAMULI8I8S(.vv/.vx).AQ`.                            *
	 *******************************************************/
}

}  // namespace simd
}  // namespace acal_lab
