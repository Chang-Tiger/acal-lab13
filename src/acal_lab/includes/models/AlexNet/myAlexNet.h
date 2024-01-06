#ifndef _MY_ALEXNET_H_
#define _MY_ALEXNET_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "acal_"
#include "acal_lab/includes/info/QuantiInfo.h"
#include "acal_lab/includes/info/TensorInfo.h"

void myAlexNet(myConvInfo *, myGemmInfo *, myMxPlInfo *, myTensorInfo *, myQuantiInfo *);

#endif
