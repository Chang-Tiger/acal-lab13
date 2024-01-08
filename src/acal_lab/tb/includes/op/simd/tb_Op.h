#ifndef __TB_OP_TB_OP_H__
#define __TB_OP_TB_OP_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "acal_lab/includes/info.h"

#define _8_BIT_ 256
#define _16_BIT_ 65536

namespace acal_lab
{
namespace tb
{
typedef enum TestType
{
    LAB,
    HW
} testType;

bool compare8(tensorInfo *, tensorInfo *);
bool compare16(tensorInfo *, tensorInfo *);
void randomInit8(tensorInfo *);
void randomInit16(tensorInfo *);
} // namespace tb
} // namespace acal_lab

#endif // __TB_OP_TB_OP_H__