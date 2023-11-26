#include "mySimdInst.h"

// 8-2.1   : Integer Extension

/******************************************************************************
 *                       8-2.1-1 : Signed Integer Addition                    *
 ******************************************************************************/
// 8-2.1-1.1 : Signed Integer Addition : sADDI8S_vv
// inline void sADDI8S_vv(int8_t c[4], int8_t a[4], int8_t b[4])
// {
//     *(int32_t *)c = cfu_op(0b000, 0b0000000, *(int32_t *)a, *(int32_t *)b);
// }

// 8-2.1-1.2 : Signed Integer Addition : sADDI16S_vv
// inline void sADDI16S_vv(int16_t c[2], int16_t a[2], int16_t b[2])
// {
//     *(int32_t *)c = cfu_op(0b001, 0b0000000, *(int32_t *)a, *(int32_t *)b);
// }

// 8-2.1-1.3 : Signed Integer Addition : sADDI8S_vx
// inline void sADDI8S_vx(int8_t c[4], int8_t a[4], int8_t b)
// {
//     *(int32_t *)c = cfu_op(0b000, 0b1000000, *(int32_t *)a, (int32_t)b);
// }

// 8-2.1-1.4 : Signed Integer Addition : sADDI16S_vx
// inline void sADDI16S_vx(int16_t c[2], int16_t a[2], int16_t b)
// {
//     *(int32_t *)c = cfu_op(0b001, 0b1000000, *(int32_t *)a, (int32_t)b);
// }

/******************************************************************************
 *                     8-2.1-2 : Signed Integer Substration                   *
 ******************************************************************************/
// 8-2.1-2.1 : Signed Integer Substration : sSUBI8S_vv
// inline void sSUBI8S_vv(int8_t c[4], int8_t a[4], int8_t b[4])
// {
//     *(int32_t *)c = cfu_op(0b000, 0b0000001, *(int32_t *)a, *(int32_t *)b);
// }

// 8-2.1-2.2 : Signed Integer Substration : sSUBI16S_vv
// inline void sSUBI16S_vv(int16_t c[2], int16_t a[2], int16_t b[2])
// {
//     *(int32_t *)c = cfu_op(0b001, 0b0000001, *(int32_t *)a, *(int32_t *)b);
// }

// 8-2.1-2.3 : Signed Integer Substration : sSUBI8S_vx
// inline void sSUBI8S_vx(int8_t c[4], int8_t a[4], int8_t b)
// {
//     *(int32_t *)c = cfu_op(0b000, 0b1000001, *(int32_t *)a, (int32_t)b);
// }

// 8-2.1-2.4 : Signed Integer Substration : sSUBI16S_vx
// inline void sSUBI16S_vx(int16_t c[2], int16_t a[2], int16_t b)
// {
//     *(int32_t *)c = cfu_op(0b001, 0b1000001, *(int32_t *)a, (int32_t)b);
// }

/******************************************************************************
 *                   8-2.1-3 : Signed Integer Multiplication                  *
 ******************************************************************************/
// 8-2.1-3.1 : Signed Integer Multiplication : sMULI8I8S_vv
// inline void sMULI8I8S_vv(int8_t c[4], int8_t a[4], int8_t b[4])
// {
//     *(int32_t *)c = cfu_op(0b000, 0b0000010, *(int32_t *)a, *(int32_t *)b);
// }

// 8-2.1-3.2 : Signed Integer Multiplication : sMULI8I16S_vv_L
// inline void sMULI8I16S_vv_L(int16_t c[2], int8_t a[4], int8_t b[4])
// {
//     *(int32_t *)c = cfu_op(0b100, 0b0000010, *(int32_t *)a, *(int32_t *)b);
// }

// 8-2.1-3.3 : Signed Integer Multiplication : sMULI8I16S_vv_H
// inline void sMULI8I16S_vv_H(int16_t c[2], int8_t a[4], int8_t b[4])
// {
//     *(int32_t *)c = cfu_op(0b101, 0b0000010, *(int32_t *)a, *(int32_t *)b);
// }

// 8-2.1-3.4 : Signed Integer Multiplication : sMULI8I8S_vx
// inline void sMULI8I8S_vx(int8_t c[4], int8_t a[4], int8_t b)
// {
//     *(int32_t *)c = cfu_op(0b000, 0b1000010, *(int32_t *)a, (int32_t)b);
// }

// 8-2.1-3.5 : Signed Integer Multiplication : sMULI8I16S_vx_L
// inline void sMULI8I16S_vx_L(int16_t c[2], int8_t a[4], int8_t b)
// {
//     *(int32_t *)c = cfu_op(0b100, 0b1000010, *(int32_t *)a, (int32_t)b);
// }

// 8-2.1-3.6 : Signed Integer Multiplication : sMULI8I16S_vx_H
// inline void sMULI8I16S_vx_H(int16_t c[2], int8_t a[4], int8_t b)
// {
//     *(int32_t *)c = cfu_op(0b101, 0b1000010, *(int32_t *)a, (int32_t)b);
// }

// 8-2.1-3.6 : Signed Integer Multiplication : sMULI8I16S_vv / sMULI8I16S_vx
inline void sMULI8I16S_vv(int16_t c[4], int8_t a[4], int8_t b[4])
{
    sMULI8I16S_vv_L(c, a, b);
    sMULI8I16S_vv_H(c + 2, a, b);
}
inline void sMULI8I16S_vx(int16_t c[2], int8_t a[4], int8_t b)
{
    sMULI8I16S_vx_L(c, a, b);
    sMULI8I16S_vx_H(c + 2, a, b);
}
