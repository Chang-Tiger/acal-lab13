#ifndef _SIMD_INST_H_
#define _SIMD_INST_H_

#include "cfu.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus

extern "C"
{
#endif

#ifndef _SIMD_INST_
#define _SIMD_INST_

// 8-2.1   : Integer Extension
/********************************************************************************************************************
 *                                          8-2.1-1 : Signed Integer Addition                                       *
 ********************************************************************************************************************/
// 8-2.1-1.1 : Signed Integer Addition : sADDI8S_vv
// void sADDI8S_vv(int8_t c[4], int8_t a[4], int8_t b[4]);
#define sADDI8S_vv(arrI8c, arrI8a, arrI8b)                                                                             \
    (*(int32_t *)(arrI8c) = cfu_op(0b000, 0b0000000, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))

// 8-2.1-1.2 : Signed Integer Addition : sADDI16S_vv
// void sADDI16S_vv(int16_t c[2], int16_t a[2], int16_t b[2]);
#define sADDI16S_vv(arrI16c, arrI16a, arrI16b)                                                                         \
    (*(int32_t *)(arrI16c) = cfu_op(0b001, 0b0000000, *(int32_t *)(arrI16a), *(int32_t *)(arrI16b)))

// 8-2.1-1.3 : Signed Integer Addition : sADDI8S_vx
// void sADDI8S_vx(int8_t c[4], int8_t a[4], int8_t b);
#define sADDI8S_vx(arrI8c, arrI8a, I8b)                                                                                \
    (*(int32_t *)(arrI8c) = cfu_op(0b000, 0b1000000, *(int32_t *)(arrI8a), (int32_t)(I8b)))

// 8-2.1-1.4 : Signed Integer Addition : sADDI16S_vx
// void sADDI16S_vx(int16_t c[2], int16_t a[2], int16_t b);
#define sADDI16S_vx(arrI16c, arrI16a, I16b)                                                                            \
    (*(int32_t *)(arrI16c) = cfu_op(0b001, 0b1000000, *(int32_t *)(arrI16a), (int32_t)(I16b)))

/********************************************************************************************************************
 *                                        8-2.1-2 : Signed Integer Substration                                      *
 ********************************************************************************************************************/
// 8-2.1-2.1 : Signed Integer Substration : sSUBI8S_vv
// void sSUBI8S_vv(int8_t c[4], int8_t a[4], int8_t b[4]);
#define sSUBI8S_vv(arrI8c, arrI8a, arrI8b)                                                                             \
    (*(int32_t *)(arrI8c) = cfu_op(0b000, 0b0000001, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))

// 8-2.1-2.2 : Signed Integer Substration : sSUBI16S_vv
// void sSUBI16S_vv(int16_t c[2], int16_t a[2], int16_t b[2]);
#define sSUBI16S_vv(arrI16c, arrI16a, arrI16b)                                                                         \
    (*(int32_t *)(arrI16c) = cfu_op(0b001, 0b0000001, *(int32_t *)(arrI16a), *(int32_t *)(arrI16b)))

// 8-2.1-2.3 : Signed Integer Substration : sSUBI8S_vx
// void sSUBI8S_vx(int8_t c[4], int8_t a[4], int8_t b);
#define sSUBI8S_vx(arrI8c, arrI8a, I8b)                                                                                \
    (*(int32_t *)(arrI8c) = cfu_op(0b000, 0b1000001, *(int32_t *)(arrI8a), (int32_t)(I8b)))

// 8-2.1-2.4 : Signed Integer Substration : sSUBI16S_vx
// void sSUBI16S_vx(int16_t c[2], int16_t a[2], int16_t b);
#define sSUBI16S_vx(arrI16c, arrI16a, I16b)                                                                            \
    (*(int32_t *)(arrI16c) = cfu_op(0b001, 0b1000001, *(int32_t *)(arrI16a), (int32_t)(I16b)))

/********************************************************************************************************************
 *                                      8-2.1-3 : Signed Integer Multiplication                                     *
 ********************************************************************************************************************/
// 8-2.1-3.1 : Signed Integer Multiplication : sMULI8I8S_vv
// void sMULI8I8S_vv(int8_t c[4], int8_t a[4], int8_t b[4]);
#define sMULI8I8S_vv(arrI8c, arrI8a, arrI8b)                                                                           \
    (*(int32_t *)(arrI8c) = cfu_op(0b000, 0b0000010, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))

// 8-2.1-3.2 : Signed Integer Multiplication : sMULI8I16S_vv_L
// void sMULI8I16S_vv_L(int16_t c[2], int8_t a[4], int8_t b[4]);
#define sMULI8I16S_vv_L(arrI16c, arrI8a, arrI8b)                                                                       \
    (*(int32_t *)(arrI16c) = cfu_op(0b100, 0b0000010, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))

// 8-2.1-3.3 : Signed Integer Multiplication : sMULI8I16S_vv_H
// void sMULI8I16S_vv_H(int16_t c[2], int8_t a[4], int8_t b[4]);
#define sMULI8I16S_vv_H(arrI16c, arrI8a, arrI8b)                                                                       \
    (*(int32_t *)(arrI16c) = cfu_op(0b101, 0b0000010, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))

// 8-2.1-3.4 : Signed Integer Multiplication : sMULI8I8S_vx
// void sMULI8I8S_vx(int8_t c[4], int8_t a[4], int8_t b);
#define sMULI8I8S_vx(arrI8c, arrI8a, I8b)                                                                              \
    (*(int32_t *)(arrI8c) = cfu_op(0b000, 0b1000010, *(int32_t *)(arrI8a), (int32_t)(I8b)))

// 8-2.1-3.5 : Signed Integer Multiplication : sMULI8I16S_vx_L
// void sMULI8I16S_vx_L(int16_t c[2], int8_t a[4], int8_t b);
#define sMULI8I16S_vx_L(arrI16c, arrI8a, I8b)                                                                          \
    (*(int32_t *)(arrI16c) = cfu_op(0b100, 0b1000010, *(int32_t *)(arrI8a), (int32_t)(I8b)))

// 8-2.1-3.6 : Signed Integer Multiplication : sMULI8I16S_vx_H
// void sMULI8I16S_vx_H(int16_t c[2], int8_t a[4], int8_t b);
#define sMULI8I16S_vx_H(arrI16c, arrI8a, I8b)                                                                          \
    (*(int32_t *)(arrI16c) = cfu_op(0b101, 0b1000010, *(int32_t *)(arrI8a), (int32_t)(I8b)))
#endif

    // 8-2.1-3.7 : Signed Integer Multiplication : sMULI8I16S_vv
    void sMULI8I16S_vv(int16_t c[4], int8_t a[4], int8_t b[4]);
    // 8-2.1-3.8 : Signed Integer Multiplication : sMULI8I16S_vx
    void sMULI8I16S_vx(int16_t c[2], int8_t a[4], int8_t b);

/********************************************************************************************************************
 *                                      8-2.1-4 : Quantization                                                      *
 ********************************************************************************************************************/
#ifndef _SIMD_QUANTIZE_
#define _SIMD_QUANTIZE_

#define sQNT_INFO(arrI8a, arrI8b) cfu_op(0b000, 0b0000111, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b))
#define sQNT_I16I8(arrI8c, arrI8a, arrI8b)                                                                             \
    (*(int32_t *)(arrI8c) = cfu_op(0b001, 0b0000111, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))

#endif

#ifdef __cplusplus
}
#endif

#endif
