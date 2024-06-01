#include "acal_lab/includes/op/simd/MxPl.h"
namespace acal_lab {
namespace simd {

void MxPl::exec() {
    int k_round;
    int8_t  temp_A[4], temp_C[4];
    int8_t  temp_B[4] = {0};
    k_round = (info->kernelSize >> 2) << 2;
    for (int c = 0; c < output->C; c++) {
		for (int oh = 0; oh < output->H; oh++) {
			for (int ow = 0; ow < output->W; ow++) {
				int    input_start_h = oh * info->stride - info->padding;
				int    input_start_w = ow * info->stride - info->padding;
				int8_t max_val       = INT8_MIN;

                int    input_end_h = input_start_h + info->kernelSize - 1;
                int    input_end_w = input_start_w + info->kernelSize - 1;
                //do parallize
                if(input_start_h >= 0 && input_end_h < input->H && input_start_w >= 0 && input_end_w < input->W){
                    for (int kh = 0; kh < info->kernelSize; kh++) {
                        for (int kw = 0; kw < k_round; kw += 4) {
                            int ih = input_start_h + kh;
                            int iw = input_start_w + kw;

                            temp_A[0] = input->data[c * input->H * input->W + ih * input->W + iw];
                            temp_A[1] = input->data[c * input->H * input->W + ih * input->W + (iw + 1)];
                            temp_A[2] = input->data[c * input->H * input->W + ih * input->W + (iw + 2)];
                            temp_A[3] = input->data[c * input->H * input->W + ih * input->W + (iw + 3)];

                            sMAXPLI8I8S_vx(temp_C, temp_A, temp_B);
                            
                            max_val = (temp_C[0] > max_val) ? temp_C[0] : max_val;
                            
                        }
                        for (int kw = k_round; kw < info->kernelSize; kw++) {//remain
                            int ih = input_start_h + kh;
                            int iw = input_start_w + kw;

                            if (ih >= 0 && ih < input->H && iw >= 0 && iw < input->W) {
                                int8_t val = input->data[c * input->H * input->W + ih * input->W + iw];
                                max_val    = (val > max_val) ? val : max_val;
                            }
                        }
                    }
                //not do
                }else{
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
                }
				

				output->data[c * output->H * output->W + oh * output->W + ow] = max_val;
			}
		}
	}
}
    
}  // namespace simd
}  // namespace acal_lab
