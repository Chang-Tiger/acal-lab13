#include "acal_lab/includes/op/scalar/ReLU.h"
namespace acal_lab {
namespace scalar {

void ReLU::exec() {
	int c_idx = 0, h_idx = 0, w_idx = 0;
	for (int c = 0; c < input->C; c++) {
		c_idx = c * input->H;
		for (int h = 0; h < input->H; h++) {
			h_idx = c_idx + h * input->W;
			for (int w = 0; w < input->W; w++) {
				w_idx               = h_idx + w;
				output->data[w_idx] = input->data[w_idx] > 0 ? input->data[w_idx] : 0;
			}
		}
	}
}

}  // namespace scalar
}  // namespace acal_lab
