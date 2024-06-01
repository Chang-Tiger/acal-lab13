#include "acal_lab/tb/includes/op/simd/tb_MxPl.h"

#define TB_SIZE 100
#include "perf.h"
#define MXPL_IPT_C       16
#define MXPL_IPT_H       56
#define MXPL_IPT_W       56
#define MXPL_KERNEL_SIZE 2
#define MXPL_STRIDE      2
#define MXPL_PADDING     0
#define MXPL_OPT_C       16
#define MXPL_OPT_H       28
#define MXPL_OPT_W       28

namespace acal_lab {
namespace tb {

bool tb_MxPl() {
	int    correct_cnt                                   = 0;
	int    tb_idx                                        = TB_SIZE;
	int8_t ipt[MXPL_IPT_C * MXPL_IPT_H * MXPL_IPT_W]     = {0};
	int8_t opt[MXPL_OPT_C * MXPL_OPT_H * MXPL_OPT_W]     = {0};
	int8_t optTest[MXPL_OPT_C * MXPL_OPT_H * MXPL_OPT_W] = {0};
	unsigned int start, start_simd, end, end_simd;
	unsigned int cpu_time_used_simd, cpu_time_used;
	cpu_time_used = 0;
	cpu_time_used_simd = 0;

	tensorInfo data_0      = {.N = 1, .C = MXPL_IPT_C, .H = MXPL_IPT_H, .W = MXPL_IPT_W, .data = ipt};
	tensorInfo data_1      = {.N = 1, .C = MXPL_OPT_C, .H = MXPL_OPT_H, .W = MXPL_OPT_W, .data = opt};
	tensorInfo data_1_Test = {.N = 1, .C = MXPL_OPT_C, .H = MXPL_OPT_H, .W = MXPL_OPT_W, .data = optTest};
	mxPlInfo   mInfo       = {.kernelSize = MXPL_KERNEL_SIZE, .stride = MXPL_STRIDE, .padding = MXPL_PADDING};

	while (tb_idx--) {
		randomInit8(&data_0);
		start_simd = perf_get_mcycle();
		simd::MxPl(&data_1, &data_0, &mInfo, GENERAL).execute();
		end_simd = perf_get_mcycle();
		cpu_time_used_simd += (end_simd - start_simd);

		start = perf_get_mcycle();
		scalar::MxPl(&data_1_Test, &data_0, &mInfo, GENERAL).execute();
		end = perf_get_mcycle();
		cpu_time_used += (end - start);
		correct_cnt += compare8(&data_1, &data_1_Test);
	}

	printf("[ TEST ] `MxPl`  :                                    %3d/%3d\n", correct_cnt, TB_SIZE);
	printf("[ TEST ] `Mxpl`  :exe time:  simd %u cycles/ scalar %u cycles\n", cpu_time_used_simd, cpu_time_used);
	return correct_cnt == TB_SIZE ? true : false;
}

}  // namespace tb
}  // namespace acal_lab
