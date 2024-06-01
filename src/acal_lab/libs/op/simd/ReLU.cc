#include "acal_lab/includes/op/simd/ReLU.h"
namespace acal_lab {
namespace simd {

void ReLU::exec() {
    int c_idx = 0, h_idx = 0, w_idx = 0;
    int8_t temp_A[4], temp_C[4];
    int8_t temp_B[4] = {0};
	for (int c = 0; c < input->C; c++) {
		c_idx = c * input->H;
		for (int h = 0; h < input->H; h++) {
			h_idx = (c_idx + h) * input->W;
            int W_round = (input->W >> 2) << 2; 
			for (int w = 0; w < W_round; w += 4) {
				w_idx               = h_idx + w;
                temp_A[0] = input->data[w_idx];
                temp_A[1] = input->data[w_idx+1];
                temp_A[2] = input->data[w_idx+2];
                temp_A[3] = input->data[w_idx+3];
                temp_B[0] = 0;
                temp_B[1] = 0;
                temp_B[2] = 0;
                temp_B[3] = 0;
                sRELUI8I8S_vx(temp_C, temp_A, temp_B);

                output->data[w_idx] = temp_C[0];
                output->data[w_idx+1] = temp_C[1];
                output->data[w_idx+2] = temp_C[2];
                output->data[w_idx+3] = temp_C[3];
			}
            for (int w = W_round; w < input->W; w++){//remain part
                w_idx               = h_idx + w;
                output->data[w_idx] = input->data[w_idx] > 0 ? input->data[w_idx] : 0;
            }
		}
	}
    
}
}  // namespace simd
}  // namespace acal_lab