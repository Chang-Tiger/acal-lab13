#include "acal_lab/includes/op/scalar/Conv.h"
namespace acal_lab {
namespace scalar {

void Conv::execPerLayerNaiveQuant() {}

void Conv::execPerLayerAdvanceQuant() {
	for (int n = 0; n < info->kernel.N; n++) {
		for (int oh = 0; oh < output->H; oh++) {
			for (int ow = 0; ow < output->W; ow++) {

				int16_t temp_ = info->bias.data[n];
				for (int c = 0; c < info->kernel.C; c++) {
					for (int kh = 0; kh < info->kernel.H; kh++) {

						int input_index = c * input->H * input->W + (oh + kh) * input->W + ow;
						int kernel_index = ((n * info->kernel.C + c) * info->kernel.H + kh) * info->kernel.W;
						for (int kw = 0; kw < info->kernel.W; kw++) {
							temp_ += (int16_t)input->data[input_index + kw] * (int16_t)info->kernel.data[kernel_index + kw];
						}
					}
				}
				output->data[n * output->H * output->W + oh * output->W + ow] = (int8_t)((temp_ >> qInfo->scaling_factor) + qInfo->zero_point);
			}
		}
	}
}

void Conv::execPerOperationNaiveQuant() {
	// Perform convolution with bias
	for (int n = 0; n < info->kernel.N; n++) {
		for (int oh = 0; oh < output->H; oh++) {
			for (int ow = 0; ow < output->W; ow++) {
				output->data[n * output->H * output->W + oh * output->W + ow] = info->bias.data[n];
				for (int c = 0; c < info->kernel.C; c++) {
					for (int kh = 0; kh < info->kernel.H; kh++) {
						for (int kw = 0; kw < info->kernel.W; kw++) {
							int16_t temp =
							    (int16_t)input->data[c * input->H * input->W + (oh + kh) * input->W + (ow + kw)] *
							    (int16_t)info->kernel
							        .data[((n * info->kernel.C + c) * info->kernel.H + kh) * info->kernel.W + kw];
							output->data[n * output->H * output->W + oh * output->W + ow] += (int8_t)(temp >> 8);
						}
					}
				}
			}
		}
	}
}

void Conv::execPerOperationAdvanceQuant() {}

}  // namespace scalar
}  // namespace acal_lab
