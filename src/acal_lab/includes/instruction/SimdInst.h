#ifndef _SIMD_INST_H_
#define _SIMD_INST_H_
#pragma GCC optimize("-fno-strict-aliasing")

#include <stdint.h>
#include <stdlib.h>

// CFU Framework
#include "cfu.h"

// 13-2.1   : Integer Extension
/******************************************************************************
 *                       13-2.1-1 : Signed Integer Addition                    *
 ******************************************************************************/
// 13-2.1-1.1 : Signed Integer Addition : sADDI8I8S_vv
// inline void sADDI8I8S_vv(int8_t c[4], int8_t a[4], int8_t b[4])
#define sADDI8I8S_vv(arrI8c, arrI8a, arrI8b)                                                                           \
    (*(int32_t *)(arrI8c) = cfu_op(0b000, 0b0000000, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))

// 13-2.1-1.2 : Signed Integer Addition : sADDI16I16S_vv
// inline void sADDI16I16S_vv(int16_t c[2], int16_t a[2], int16_t b[2])
#define sADDI16I16S_vv(arrI16c, arrI16a, arrI16b)                                                                      \
    (*(int32_t *)(arrI16c) = cfu_op(0b001, 0b0000000, *(int32_t *)(arrI16a), *(int32_t *)(arrI16b)))

// 13-2.1-1.3 : Signed Integer Addition : sADDI8I8S_vx
// inline void sADDI8I8S_vx(int8_t c[4], int8_t a[4], int8_t b)
#define sADDI8I8S_vx(arrI8c, arrI8a, I8b)                                                                              \
    (*(int32_t *)(arrI8c) = cfu_op(0b000, 0b1000000, *(int32_t *)(arrI8a), (int32_t)(I8b)))

// 13-2.1-1.4 : Signed Integer Addition : sADDI16I16S_vx
// inline void sADDI16I16S_vx(int16_t c[2], int16_t a[2], int16_t b)
#define sADDI16I16S_vx(arrI16c, arrI16a, I16b)                                                                         \
    (*(int32_t *)(arrI16c) = cfu_op(0b001, 0b1000000, *(int32_t *)(arrI16a), (int32_t)(I16b)))

/******************************************************************************
 *                     13-2.1-2 : Signed Integer Substration                   *
 ******************************************************************************/
// 13-2.1-2.1 : Signed Integer Substration : sSUBI8I8S_vv
// inline void sSUBI8I8S_vv(int8_t c[4], int8_t a[4], int8_t b[4])
#define sSUBI8I8S_vv(arrI8c, arrI8a, arrI8b)                                                                           \
    (*(int32_t *)(arrI8c) = cfu_op(0b000, 0b0000001, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))

// 13-2.1-2.2 : Signed Integer Substration : sSUBI16I16S_vv
// inline void sSUBI16I16S_vv(int16_t c[2], int16_t a[2], int16_t b[2])
#define sSUBI16I16S_vv(arrI16c, arrI16a, arrI16b)                                                                      \
    (*(int32_t *)(arrI16c) = cfu_op(0b001, 0b0000001, *(int32_t *)(arrI16a), *(int32_t *)(arrI16b)))

// 13-2.1-2.3 : Signed Integer Substration : sSUBI8I8S_vx
// inline void sSUBI8I8S_vx(int8_t c[4], int8_t a[4], int8_t b)
#define sSUBI8I8S_vx(arrI8c, arrI8a, I8b)                                                                              \
    (*(int32_t *)(arrI8c) = cfu_op(0b000, 0b1000001, *(int32_t *)(arrI8a), (int32_t)(I8b)))

// 13-2.1-2.4 : Signed Integer Substration : sSUBI16I16S_vx
// inline void sSUBI16I16S_vx(int16_t c[2], int16_t a[2], int16_t b)
#define sSUBI16I16S_vx(arrI16c, arrI16a, I16b)                                                                         \
    (*(int32_t *)(arrI16c) = cfu_op(0b001, 0b1000001, *(int32_t *)(arrI16a), (int32_t)(I16b)))

/******************************************************************************
 *        13-2.1-3 : Signed Integer Precise(Lossless) Multiplication           *
 ******************************************************************************/
// 13-2.1-3.1 : Signed Integer Precise(Lossless) Multiplication :
// sPMULI8I16S_vv_L inline void sPMULI8I16S_vv_L(int16_t c[2], int8_t a[4],
// int8_t b[4])
#define sPMULI8I16S_vv_L(arrI16c, arrI8a, arrI8b)                                                                      \
    (*(int32_t *)(arrI16c) = cfu_op(0b100, 0b0000010, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))

// 13-2.1-3.2 : Signed Integer Precise(Lossless) Multiplication :
// sPMULI8I16S_vv_H inline void sPMULI8I16S_vv_H(int16_t c[2], int8_t a[4],
// int8_t b[4])
#define sPMULI8I16S_vv_H(arrI16c, arrI8a, arrI8b)                                                                      \
    (*(int32_t *)(arrI16c) = cfu_op(0b101, 0b0000010, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))

// 13-2.1-3.4 : Signed Integer Precise(Lossless) Multiplication :
// sPMULI8I16S_vx_L inline void sPMULI8I16S_vx_L(int16_t c[2], int8_t a[4],
// int8_t b)
#define sPMULI8I16S_vx_L(arrI16c, arrI8a, I8b)                                                                         \
    (*(int32_t *)(arrI16c) = cfu_op(0b100, 0b1000010, *(int32_t *)(arrI8a), (int32_t)(I8b)))

// 13-2.1-3.5 : Signed Integer Precise(Lossless) Multiplication :
// sPMULI8I16S_vx_H inline void sPMULI8I16S_vx_H(int16_t c[2], int8_t a[4],
// int8_t b)
#define sPMULI8I16S_vx_H(arrI16c, arrI8a, I8b)                                                                         \
    (*(int32_t *)(arrI16c) = cfu_op(0b101, 0b1000010, *(int32_t *)(arrI8a), (int32_t)(I8b)))

/******************************************************************************
 *         13-2.1-4 : Signed Integer Approximate(Lossy) Multiplication         *
 ******************************************************************************/
// 13-2.1-4.1 : Signed Integer Approximate(Lossy) MUL w/ Naive Quantization :
// sAMULI8I8S_vv_NQ inline void sAMULI8I8S_vv_NQ(int8_t c[4], int8_t a[4],
// int8_t b[4])
#define sAMULI8I8S_vv_NQ(arrI8c, arrI8a, arrI8b)                                                                       \
    (*(int32_t *)(arrI8c) = cfu_op(0b000, 0b0000010, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))

// 13-2.1-4.2 : Signed Integer Approximate(Lossy) MUL w/ Advance Quantization :
// sAMULI8I8S_vv_AQ inline void sAMULI8I8S_vv_AQ(int8_t c[4], int8_t a[4],
// int8_t b[4])
#define sAMULI8I8S_vv_AQ(arrI8c, arrI8a, arrI8b)                                                                       \
    (*(int32_t *)(arrI8c) = cfu_op(0b001, 0b0000010, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))

#define sAMULI8I8S_vx_NQ(arrI8c, arrI8a, I8b)                                                                          \
    (*(int32_t *)(arrI8c) = cfu_op(0b000, 0b1000010, *(int32_t *)(arrI8a), (int32_t)(I8b)))

// 13-2.1-4.4 : Signed Integer Approximate(Lossy) MUL w/ Advance Quantization :
// sAMULI8I8S_vx_AQ inline void sAMULI8I8S_vx_AQ(int8_t c[4], int8_t a[4],
// int8_t b[4])
#define sAMULI8I8S_vx_AQ(arrI8c, arrI8a, I8b)                                                                          \
    (*(int32_t *)(arrI8c) = cfu_op(0b001, 0b1000010, *(int32_t *)(arrI8a), (int32_t)(I8b)))

/******************************************************************************
 *                           13-2.1-4 : Quantization                           *
 ******************************************************************************/
// 13-2.2 : Quantization
#define sQNT_INFO(scalingFactor, zeroPt) cfu_op(0b000, 0b0000111, scalingFactor, zeroPt)
#define sQNTI16I8S_vv_NQ(arrI8c, arrI8a, arrI8b)                                                                       \
    (*(int32_t *)(arrI8c) = cfu_op(0b001, 0b0000111, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))
#define sQNTI16I8S_vv_AQ(arrI8c, arrI8a, arrI8b)                                                                       \
    (*(int32_t *)(arrI8c) = cfu_op(0b010, 0b1000111, *(int32_t *)(arrI8a), *(int32_t *)(arrI8b)))

#endif