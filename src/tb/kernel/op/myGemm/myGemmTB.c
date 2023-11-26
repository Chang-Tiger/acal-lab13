#include "myGemmTB.h"
bool myGemm_Kernel()
{
    int8_t data[GEMM_dimM * GEMM_dimK] = {0};
    int8_t gemm_w[GEMM_dimK * GEMM_dimN] = {0};
    int8_t gemm_b[GEMM_dimM * GEMM_dimN] = {0};
    int8_t gemm[GEMM_dimM * GEMM_dimN] = {0};
    int8_t gemmTest[GEMM_dimM * GEMM_dimN] = {0};

    for (int m = 0; m < GEMM_dimM; m++)
    {
        for (int n = 0; n < GEMM_dimN; n++)
        {
            gemm_b[m * GEMM_dimN + n] = (int8_t)(rand() % _8_BIT_);
            for (int k = 0; k < GEMM_dimK; k++)
            {
                gemm_w[k * GEMM_dimN + n] = (int8_t)(rand() % _8_BIT_);
                data[m * GEMM_dimK + k] = (int8_t)(rand() % _8_BIT_);
            }
            gemm_b[m * dimN + n] = (int8_t)(rand() % _8_BIT_);
        }
    }

    myTensorInfo data_0 = {.C = 1, .H = GEMM_dimM, .W = GEMM_dimK, .data = data};
    myTensorInfo gemm1_w_0 = {.C = 1, .H = GEMM_dimK, .W = GEMM_dimN, .data = gemm_w};
    myTensorInfo gemm1_b_0 = {.C = 1, .H = GEMM_dimM, .W = GEMM_dimN, .data = gemm_b};
    myTensorInfo gemm1_0 = {.C = 1, .H = GEMM_dimM, .W = GEMM_dimN, .data = gemm};
    myTensorInfo gemm1_0_Test = {.C = 1, .H = GEMM_dimM, .W = GEMM_dimN, .data = gemmTest};

    myGemmInfo gemm1_info = {.weight = gemm1_w_0, .bias = gemm1_b_0};
    myGemm(&gemm1_0, &data_0, &gemm1_info);
    myGemmScalar(&gemm1_0_Test, &data_0, &gemm1_info);

    // printf("----------    Matrix C     ----------\n");
    // printf("----------  SIMD Extension  ---------\n");
    // for (int m = 0; m < GEMM_dimM; m++, printf("\n"))
    //     for (int n = 0; n < GEMM_dimN; n++)
    //         printf("%4x ", gemm[m * GEMM_dimN + n] & 0x000000ff);
    // printf("---------- RV32IM Extension ---------\n");
    // for (int m = 0; m < GEMM_dimM; m++, printf("\n"))
    //     for (int n = 0; n < GEMM_dimN; n++)
    //         printf("%4x ", gemmTest[m * GEMM_dimN + n] & 0x000000ff);
    int correct = 0;
    // printf("----------     Comparation      ----------\n");
    for (int m = 0; m < GEMM_dimM; m++)
        for (int n = 0; n < GEMM_dimN; n++)
            correct += ((gemm[m * GEMM_dimN + n] & 0x000000ff) == (gemmTest[m * GEMM_dimN + n] & 0x000000ff) ? 1 : 0);
    printf("-------------------------------------\n");
    printf("[ TEST ] `GEMM`  :               %4s\n", correct == GEMM_dimM * GEMM_dimN ? "Pass" : "Fail");
    printf("-------------------------------------\n");

    return correct == GEMM_dimM * GEMM_dimN ? true : false;
}