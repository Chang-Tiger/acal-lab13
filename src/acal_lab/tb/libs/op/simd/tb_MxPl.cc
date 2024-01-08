#include "acal_lab/tb/includes/op/simd/tb_MxPl.h"

#define TB_SIZE 100



bool acal_lab::tb::tb_MxPl()
{
    int correct_cnt = 0;

    printf("[ TEST ] `MxPl`  :                                    %3d/%3d\n", correct_cnt, TB_SIZE);
    return correct_cnt == TB_SIZE ? true : false;
}