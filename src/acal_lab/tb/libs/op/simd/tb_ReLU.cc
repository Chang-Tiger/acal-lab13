#include "acal_lab/tb/includes/op/simd/tb_ReLU.h"
#include "perf.h"
#define TB_SIZE 100
namespace acal_lab {
namespace tb {

bool tb_ReLU() {
	int    correct_cnt                                = 0;
	int8_t ipt[RELU_dimC * RELU_dimH * RELU_dimW]     = {0};
	int8_t opt[RELU_dimC * RELU_dimH * RELU_dimW]     = {0};
	int8_t optTest[RELU_dimC * RELU_dimH * RELU_dimW] = {0};
	unsigned int start, start_simd, end, end_simd;
	unsigned int cpu_time_used_simd, cpu_time_used;

	tensorInfo data_0      = {.N = 1, .C = RELU_dimC, .H = RELU_dimH, .W = RELU_dimW, .data = ipt};
	tensorInfo data_1      = {.N = 1, .C = RELU_dimC, .H = RELU_dimH, .W = RELU_dimW, .data = opt};
	tensorInfo data_1_Test = {.N = 1, .C = RELU_dimC, .H = RELU_dimH, .W = RELU_dimW, .data = optTest};

	cpu_time_used = 0;
	cpu_time_used_simd = 0;
	int tb_idx = TB_SIZE;
	while (tb_idx--) {
		randomInit8(&data_0);
		start_simd = perf_get_mcycle();
		simd::ReLU(&data_1, &data_0, GENERAL).execute();
		end_simd = perf_get_mcycle();
		cpu_time_used_simd += end_simd - start_simd;

		start = perf_get_mcycle();
		scalar::ReLU(&data_1_Test, &data_0, GENERAL).execute();
		end = perf_get_mcycle();
		correct_cnt += compare8(&data_1, &data_1_Test);
		cpu_time_used += end - start;
	}

	printf("[ TEST ] `ReLU`  :                                    %3d/%3d\n", correct_cnt, TB_SIZE);
	printf("[ TEST ] `ReLU`  :exe time:  simd %u cycles/ scalar %u cycles\n", cpu_time_used_simd, cpu_time_used);

	return correct_cnt == TB_SIZE ? true : false;
}

}  // namespace tb
}  // namespace acal_lab
