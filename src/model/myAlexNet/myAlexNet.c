#include "myAlexNet.h"
#include "myAlexNetCfg.h"

void myAlexNet(myConvInfo *convInfo, myGemmInfo *gemmInfo, myMxPlInfo *mxplInfo, myTensorInfo *tnsrInfo)
{
    myConv(&tnsrInfo[1], &tnsrInfo[0], &convInfo[0]);
    myReLU(&tnsrInfo[2], &tnsrInfo[1]);
    myMxPl(&tnsrInfo[3], &tnsrInfo[2], &mxplInfo[0]);
    myConv(&tnsrInfo[4], &tnsrInfo[3], &convInfo[1]);
    myReLU(&tnsrInfo[5], &tnsrInfo[4]);
    myMxPl(&tnsrInfo[6], &tnsrInfo[5], &mxplInfo[1]);
    myConv(&tnsrInfo[7], &tnsrInfo[6], &convInfo[2]);
    myReLU(&tnsrInfo[8], &tnsrInfo[7]);
    myConv(&tnsrInfo[9], &tnsrInfo[8], &convInfo[3]);
    myReLU(&tnsrInfo[10], &tnsrInfo[9]);
    myConv(&tnsrInfo[11], &tnsrInfo[10], &convInfo[4]);
    myReLU(&tnsrInfo[12], &tnsrInfo[11]);
    myMxPl(&tnsrInfo[13], &tnsrInfo[12], &mxplInfo[2]);
    myGemm(&tnsrInfo[14], &tnsrInfo[13], &gemmInfo[0]);
    myReLU(&tnsrInfo[15], &tnsrInfo[14]);
    myGemm(&tnsrInfo[16], &tnsrInfo[15], &gemmInfo[1]);
    myReLU(&tnsrInfo[17], &tnsrInfo[16]);
    myGemm(&tnsrInfo[18], &tnsrInfo[17], &gemmInfo[2]);
    myReLU(&tnsrInfo[19], &tnsrInfo[18]);
}