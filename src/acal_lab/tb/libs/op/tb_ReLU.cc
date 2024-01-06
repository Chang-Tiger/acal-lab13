#include "acal_lab/tb/includes/op/tb_ReLU.h"

#define TB_SIZE 100

void acal_lab::tb::TB_ReLU::exec()
{
}

bool acal_lab::tb::tb_ReLU()
{
    int correct_cnt = 0;

    printf("[ TEST ] `ReLU`  :                                    %3d/%3d\n", correct_cnt, TB_SIZE);

    return correct_cnt == TB_SIZE ? true : false;
}