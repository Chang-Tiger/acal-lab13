#ifndef _MY_GEMM_TB_H_
#define _MY_GEMM_TB_H_

#include "../../kernel/includes/myTensor.h"
#include "../../kernel/op/myGemm/myGemm.h"
#include "../../simd/mySimdInst.h"
#include "cfu.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define GEMM_dimM 8
#define GEMM_dimN 8
#define GEMM_dimK 8
#define _8_BIT_ 256

#ifdef __cplusplus

extern "C"
{
#endif
    bool myGemm_Kernel();
#ifdef __cplusplus
}
#endif

#endif