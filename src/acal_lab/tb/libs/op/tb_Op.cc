#include "acal_lab/tb/includes/op/tb_Op.h"

bool acal_lab::tb::compare8(tensorInfo *hw, tensorInfo *tb)
{
    int correct = 0;

    for (int c = 0; c < hw->C; c++)
        for (int h = 0; h < hw->H; h++)
            for (int w = 0; w < hw->W; w++)
            {
                int index = ((c * hw->H) + h) * hw->W + w;
                correct += ((hw->data[index] == tb->data[index]) ? 1 : 0);
            }

    return correct == (hw->C * hw->H * hw->W) ? 1 : 0;
}

bool acal_lab::tb::compare16(tensorInfo *hw, tensorInfo *tb)
{
    int correct = 0;
    for (int c = 0; c < hw->C; c++)
        for (int h = 0; h < hw->H; h++)
            for (int w = 0; w < hw->W; w++)
            {
                int index = ((c * hw->H) + h) * hw->W + w;
                correct += ((hw->data_16[index] == tb->data_16[index]) ? 1 : 0);
            }
    return correct == (hw->C * hw->H * hw->W) ? 1 : 0;
}

void acal_lab::tb::randomInit16(acal_lab::tensorInfo *tInfo)
{
    for (int c = 0; c < tInfo->C; c++)
        for (int h = 0; h < tInfo->H; h++)
            for (int w = 0; w < tInfo->W; w++)
            {
                int index = (c * tInfo->H + h) * tInfo->W + w;
                tInfo->data_16[index] = (int16_t)(rand() % (2 * _16_BIT_) - _16_BIT_);
            }
}

void acal_lab::tb::randomInit8(acal_lab::tensorInfo *tInfo)
{
    for (int c = 0; c < tInfo->C; c++)
        for (int h = 0; h < tInfo->H; h++)
            for (int w = 0; w < tInfo->W; w++)
            {
                int index = (c * tInfo->H + h) * tInfo->W + w;
                tInfo->data_16[index] = (int8_t)(rand() % (2 * _8_BIT_) - _8_BIT_);
            }
}
