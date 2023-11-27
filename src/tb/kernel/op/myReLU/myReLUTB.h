#ifndef _MY_RELU_TB_H_
#define _MY_RELU_TB_H_

#include "../../kernel/myKernel.h"
#include "../../kernel/op/myReLU/myReLU.h"
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
    bool myReLU_Kernel();
#ifdef __cplusplus
}
#endif

#endif