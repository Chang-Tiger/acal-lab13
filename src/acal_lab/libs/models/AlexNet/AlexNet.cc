
#include "acal_lab/includes/models/AlexNet/AlexNet.h"
namespace acal_lab {

void alexNet(convInfo* cInfo, gemmInfo* gInfo, mxPlInfo* mInfo, tensorInfo* tInfo, quantiInfo* qInfo) {
	simd::Conv(&tInfo[1], &tInfo[0], &cInfo[0], &qInfo[0], PER_LAYER_ADVANCE_QUANTI).execute();
	simd::ReLU(&tInfo[2], &tInfo[1], GENERAL).execute();
	simd::MxPl(&tInfo[3], &tInfo[2], &mInfo[0], GENERAL).execute();
	simd::Conv(&tInfo[4], &tInfo[3], &cInfo[1], &qInfo[1], PER_LAYER_ADVANCE_QUANTI).execute();
	simd::ReLU(&tInfo[5], &tInfo[4], GENERAL).execute();
	simd::MxPl(&tInfo[6], &tInfo[5], &mInfo[1], GENERAL).execute();
	simd::Conv(&tInfo[7], &tInfo[6], &cInfo[2], &qInfo[2], PER_LAYER_ADVANCE_QUANTI).execute();
	simd::ReLU(&tInfo[8], &tInfo[7], GENERAL).execute();
	simd::Conv(&tInfo[9], &tInfo[8], &cInfo[3], &qInfo[3], PER_LAYER_ADVANCE_QUANTI).execute();
	simd::ReLU(&tInfo[10], &tInfo[9], GENERAL).execute();
	simd::Conv(&tInfo[11], &tInfo[10], &cInfo[4], &qInfo[4], PER_LAYER_ADVANCE_QUANTI).execute();
	simd::ReLU(&tInfo[12], &tInfo[11], GENERAL).execute();
	simd::MxPl(&tInfo[13], &tInfo[12], &mInfo[2], GENERAL).execute();
	simd::Gemm(&tInfo[14], &tInfo[13], &gInfo[0], &qInfo[6], PER_LAYER_ADVANCE_QUANTI).execute();
	simd::ReLU(&tInfo[15], &tInfo[14], GENERAL).execute();
	simd::Gemm(&tInfo[16], &tInfo[15], &gInfo[1], &qInfo[7], PER_LAYER_ADVANCE_QUANTI).execute();
	simd::ReLU(&tInfo[17], &tInfo[16], GENERAL).execute();
	simd::Gemm(&tInfo[18], &tInfo[17], &gInfo[2], &qInfo[8], PER_LAYER_ADVANCE_QUANTI).execute();
}

}  // namespace acal_lab
