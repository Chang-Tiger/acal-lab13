#include "acal_lab/includes/op/scalar/MxPl.h"
namespace acal_lab {
namespace scalar {

void MxPl::exec() {
	for (int c = 0; c < output->C; c++) {
		for (int oh = 0; oh < output->H; oh++) {
			for (int ow = 0; ow < output->W; ow++) {
				int    input_start_h = oh * info->stride - info->padding;
				int    input_start_w = ow * info->stride - info->padding;
				int8_t max_val       = INT8_MIN;

				for (int kh = 0; kh < info->kernelSize; kh++) {
					for (int kw = 0; kw < info->kernelSize; kw++) {
						int ih = input_start_h + kh;
						int iw = input_start_w + kw;

						if (ih >= 0 && ih < input->H && iw >= 0 && iw < input->W) {
							int8_t val = input->data[c * input->H * input->W + ih * input->W + iw];
							max_val    = (val > max_val) ? val : max_val;
						}
					}
				}

				output->data[c * output->H * output->W + oh * output->W + ow] = max_val;
			}
		}
	}
}

}  // namespace scalar
}  // namespace acal_lab