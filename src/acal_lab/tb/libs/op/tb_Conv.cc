#include "acal_lab/tb/includes/op/tb_Conv.h"

#include "acal_lab/includes/info.h"
#include "acal_lab/includes/op.h"

#define TB_SIZE 100
bool acal_lab::tb::tb_Conv()
{
    int correct_cnt = 0;
    printf("[ TEST ] `CONV`  :                       %3d/%3d\n", correct_cnt, TB_SIZE);

    return correct_cnt == TB_SIZE ? true : false;
}