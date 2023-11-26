
#ifndef _TB_MODEL_MY_ALEXNET_TB_H_
#define _TB_MODEL_MY_ALEXNET_TB_H_

#include "../../model/myAlexNet/myAlexNet.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus

extern "C"
{
#endif
    void myAlexNetTB();
    void param_init(myConvInfo*, myGemmInfo*);
    void input_init(myTensorInfo *);

#ifdef __cplusplus
}
#endif

#endif
