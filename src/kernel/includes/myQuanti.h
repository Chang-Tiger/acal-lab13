
#ifndef _MY_QUANTI_H_
#define _MY_QUANTI_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus

extern "C"
{
#endif
    typedef struct MyQuantiInfo
    {
        int8_t scaling_factor;
        int8_t zero_point;
    } myQuantiInfo;
#ifdef __cplusplus
}
#endif

#endif