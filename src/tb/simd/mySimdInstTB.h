#ifndef _SIMD_INST_TB_H_
#define _SIMD_INST_TB_H_

#include "../../simd/mySimdInst.h"
#include "cfu.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus

extern "C"
{
#endif
    // 8-2.1   : Integer Extension
    // 8-2.1-1 : Signed Integer Addition
    bool sADD_vv_TB();
    bool sADD_vx_TB();
    // 8-2.1-2 : Signed Integer Substration
    bool sSUB_vv_TB();
    bool sSUB_vx_TB();
    // 8-2.1-3 : Signed Integer Multiplication
    bool sMUL_vv_TB();
    bool sMUL_vx_TB();

#ifdef __cplusplus
}
#endif

#endif