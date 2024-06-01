#include "acal_lab/includes/op/simd/Gemm.h"
namespace acal_lab {
namespace simd {
inline void sPMULI8I16S_vv(int16_t c[4], int8_t a[4], int8_t b[4]) {
	sPMULI8I16S_vv_L(c, a, b);
	sPMULI8I16S_vv_H(c + 2, a, b);
}
void Gemm::execPerLayerNaiveQuant() {}

void Gemm::execPerOperationNaiveQuant() {
	int    index_A, index_B, index_C;
	int8_t temp_A[4] = {0}, temp_B[4] = {0}, temp_C[4] = {0};

	for (int m = 0; m < input->H; m++) {
		index_A = m * input->W;   // M * K
		index_C = m * output->W;  // M * N
		for (int k = 0; k < input->W; k++) {
			index_B = k * info->weight.W;  // K * N
			for (int n = 0; n < info->weight.W; n += 4) {
				for (int i = 0; i < 4; ++i) temp_A[i] = input->data[index_A + k];
				*(int32_t*)temp_B = *(int32_t*)&(info->weight.data)[index_B + n];
				int output_index  = index_C + n;
				sAMULI8I8S_vv_NQ(temp_C, temp_A, temp_B);
				sADDI8I8S_vv((int8_t*)&(output->data[output_index]), (int8_t*)&(output->data[output_index]),
				             (int8_t*)(temp_C));
			}
		}
		for (int n = 0; n < info->weight.W;) {
			sADDI8I8S_vv((int8_t*)&(output->data[index_C + n]), (int8_t*)&(output->data[index_C + n]),
			             (int8_t*)&(info->bias.data[index_C + n]));
			n += 4;
		}
	}
}

void Gemm::execPerLayerAdvanceQuant() {
	/********************************************************************
	 * TODO:                                                            *
	 * For Homework 13.3, your task is to implement GEMM with per Layer *
	 * `Advance Quantization`. This involves using the instruction      *
	 * `sPMULI8I16S(.vv/.vx)` to generate int16 output. However, the    *
	 * int16 output needs to be converted to int8 and then forwarded    *
	 * to the next Operator. To achieve this, utilize `sQNTI16I8S.vv.AQ`*
	 * for the conversion.                                              *
	 *******************************************************************/
	int8_t  temp_A[4], temp_B[4];
	int16_t temp_C[4];
	int16_t tempINT16_Buffer[10000] = {0};
	sQNT_INFO(qInfo->scaling_factor, qInfo->zero_point);
	//int remain = info->weight.W % 4
	int n_round, index_A, index_B, index_C;
	n_round = (info->weight.W >> 2) << 2; 
	for (int m = 0; m < input->H; m++) {
		index_A = m * input->W;   // M * K
		index_C = m * output->W;  // M * N
		for (int k = 0; k < input->W; k++) {
			index_B = k * info->weight.W;  // K * N


			for (int n = 0; n < n_round; n += 4) {
				for (int i = 0; i < 4; ++i) { temp_A[i] = input->data[index_A + k];}
				temp_B[0] = info->weight.data[index_B + n];
				temp_B[1] = info->weight.data[index_B + n + 1];
				temp_B[2] = info->weight.data[index_B + n + 2];
				temp_B[3] = info->weight.data[index_B + n + 3];
				//int output_index  = index_C + n;

				// SIMD MUL
				sPMULI8I16S_vv(temp_C, temp_A, temp_B);
				tempINT16_Buffer[index_C + n] += temp_C[0];
				tempINT16_Buffer[index_C + n + 1] += temp_C[1];
				tempINT16_Buffer[index_C + n + 2] += temp_C[2];
				tempINT16_Buffer[index_C + n + 3] += temp_C[3];
			}
			for (int n = n_round; n < info->weight.W; n++){//remain part
				tempINT16_Buffer[index_C + n] +=
				    (int16_t)input->data[index_A + k] * (int16_t)info->weight.data[index_B + n];
			}
		}
		for (int n = 0; n < info->weight.W; n++) { tempINT16_Buffer[index_C + n] += info->bias.data[index_C + n];}
	}
	// PER LAYER QUANTIZATION
	int tempH = 0, tempW = 0;
	int output_w_round = (output->W >> 2) << 2; 
	for (int h = 0; h < output->H; h++) {
		tempH = h * output->W;  // M * N
		for (int w = 0; w < output_w_round; w += 4) {
			tempW = tempH + w;
			temp_C[0] = tempINT16_Buffer[tempW];
			temp_C[1] = tempINT16_Buffer[tempW+1];
			temp_C[2] = tempINT16_Buffer[tempW+2];
			temp_C[3] = tempINT16_Buffer[tempW+3];
			sQNTI16I8S_vv_AQ(temp_A, temp_C, temp_C + 2);
			output->data[tempW] = temp_A[0];
			output->data[tempW + 1] = temp_A[1];
			output->data[tempW + 2] = temp_A[2];
			output->data[tempW + 3] = temp_A[3];
		}
		for (int w = output_w_round; w < output->W; w++){//remain part
			tempW = tempH + w;
			output->data[tempW] = (int8_t)((tempINT16_Buffer[tempW] >> qInfo->scaling_factor) + qInfo->zero_point);
		}
	}
}

void Gemm::execPerOperationAdvanceQuant() {
	/********************************************************************
	 * TODO:                                                            *
	 * For Homework 13.3, your task is to implement GEMM with per Layer *
	 * `Advance Quantization`. This involves using the instruction      *
	 * `sPMULI8I16S(.vv/.vx)` to generate int16 output. However, the    *
	 * int16 output needs to be converted to int8 and then forwarded    *
	 * to the next Operator. To achieve this, utilize `sQNTI16I8S.vv.AQ`*
	 * for the conversion.                                              *
	 *******************************************************************/
}

}  // namespace simd
}  // namespace acal_lab
