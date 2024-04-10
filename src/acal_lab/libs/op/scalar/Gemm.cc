#include "acal_lab/includes/op/scalar/Gemm.h"
namespace acal_lab {
namespace scalar {

void Gemm::execPerOperationNaiveQuant() {
	int8_t temp_A[4] = {0}, temp_B[4] = {0};
	int    index_A, index_B, index_C;
	for (int m = 0; m < input->H; m++) {
		index_A = m * input->W;   // M * K
		index_C = m * output->W;  // M * N
		for (int k = 0; k < input->W; k++) {
			index_B = k * info->weight.W;  // K * N
			for (int n = 0; n < info->weight.W; n += 4) {
				*(int32_t*)temp_A = (int32_t)0x01010101 * (int32_t)input->data[index_A + k];
				*(int32_t*)temp_B = *(int32_t*)&(info->weight.data)[index_B + n];
				int output_index  = index_C + n;
				for (int i = 0; i < 4; i++, output_index++) {
					int16_t temp = (int16_t)temp_A[i] * (int16_t)temp_B[i];
					temp         = temp >> 8;
					output->data[output_index] += temp;
				}
			}
		}
		for (int n = 0; n < info->weight.W; n++) output->data[index_C + n] += info->bias.data[index_C + n];
	}
}

void Gemm::execPerOperationAdvanceQuant() {
	int8_t temp_A[4] = {0}, temp_B[4] = {0};
	int    index_A, index_B, index_C;
	for (int m = 0; m < input->H; m++) {
		index_A = m * input->W;   // M * K
		index_C = m * output->W;  // M * N
		for (int k = 0; k < input->W; k++) {
			index_B = k * info->weight.W;  // K * N
			for (int n = 0; n < info->weight.W; n += 4) {
				*(int32_t*)temp_A = (int32_t)0x01010101 * (int32_t)input->data[index_A + k];
				*(int32_t*)temp_B = *(int32_t*)&(info->weight.data)[index_B + n];
				int output_index  = index_C + n;
				for (int i = 0; i < 4; i++, output_index++) {
					int16_t temp = (int16_t)temp_A[i] * (int16_t)temp_B[i];
					temp         = (temp >> qInfo->scaling_factor) + qInfo->zero_point;
					output->data[output_index] += temp;
				}
			}
		}
		for (int n = 0; n < info->weight.W; n++) output->data[index_C + n] += info->bias.data[index_C + n];
	}
}

void Gemm::execPerLayerNaiveQuant() {
	int16_t tempINT16_Buffer[10000] = {0};

	int8_t temp_A[4] = {0}, temp_B[4] = {0};
	int    index_A, index_B, index_C;
	for (int m = 0; m < input->H; m++) {
		index_A = m * input->W;   // M * K
		index_C = m * output->W;  // M * N
		for (int k = 0; k < input->W; k++) {
			index_B = k * info->weight.W;  // K * N
			for (int n = 0; n < info->weight.W; n += 4) {
				*(int32_t*)temp_A = (int32_t)0x01010101 * (int32_t)input->data[index_A + k];
				*(int32_t*)temp_B = *(int32_t*)&(info->weight.data)[index_B + n];
				int output_index  = index_C + n;
				for (int i = 0; i < 4; i++, output_index++) {
					tempINT16_Buffer[output_index] += (int16_t)((int32_t)temp_A[i]) * ((int32_t)temp_B[i]);
				}
			}
		}
		for (int n = 0; n < info->weight.W; n++) tempINT16_Buffer[index_C + n] += (int16_t)info->bias.data[index_C + n];
	}
	// PER LAYER QUANTIZATION
	int tempH = 0, tempW;
	for (int h = 0; h < output->H; h++) {
		tempH = h * output->W;  // M * N
		for (int w = 0; w < input->W; w++) {
			tempW               = tempH + w;
			output->data[tempW] = (int8_t)(tempINT16_Buffer[tempW] >> 8);
		}
	}
}
void Gemm::execPerLayerAdvanceQuant() {}

}  // namespace scalar
}  // namespace acal_lab
