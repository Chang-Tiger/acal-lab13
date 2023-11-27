#ifndef _MY_CONV_TB_H_
#define _MY_CONV_TB_H_

#include "../../kernel/myKernel.h"
#include "../../kernel/op/myConv/myConv.h"
#include "../../simd/mySimdInst.h"
#include "cfu.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif
    bool myConv_Kernel();
#ifdef __cplusplus
}
#endif

#endif