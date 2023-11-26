#include "mySimdInstTB.h"

#define TB_DATA_SIZE_8_BIT 16
#define TB_DATA_SIZE_16_BIT 256
#define TB_SIZE 100

/********************************************************************************************************************
 *                                          8-2.1-1 : Signed Integer Addition                                       *
 ********************************************************************************************************************/
bool sADD_vv_TB()
{
    int8_t temp_8_rs1[4], temp_8_rs2[4], temp_8_rd[4], temp_8_rd_tb[4];
    int16_t temp_16_rs1[2], temp_16_rs2[2], temp_16_rd[2], temp_16_rd_tb[2];
    int tb_idx;

    // 8-2.1-1.1 : Signed Integer Addition : sADDI8S_vv
    // void sADDI8S_vv(int8_t c[4], int8_t a[4], int8_t b[4]);
    tb_idx = TB_SIZE;
    int addi8s_vv_cnt = 0;
    while (tb_idx--)
    {
        int idx = 4;
        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_8_rs2[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_8_rd_tb[idx] = temp_8_rs1[idx] + temp_8_rs2[idx];
        }
        sADDI8S_vv(temp_8_rd, temp_8_rs1, temp_8_rs2);
        addi8s_vv_cnt += ((temp_8_rd[0] == temp_8_rd_tb[0]) & (temp_8_rd[1] == temp_8_rd_tb[1]) &
                          (temp_8_rd[2] == temp_8_rd_tb[2]) & (temp_8_rd[3] == temp_8_rd_tb[3]))
                             ? 1
                             : 0;
    }

    // 8-2.1-1.2 : Signed Integer Addition : sADDI16S_vv
    // void sADDI16S_vv(int16_t c[2], int16_t a[2], int16_t b[2]);
    tb_idx = TB_SIZE;
    int addi16s_vv_cnt = 0;
    while (tb_idx--)
    {
        int idx = 2;
        while (idx--)
        {
            temp_16_rd[idx] = 0;
            temp_16_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rs2[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rd_tb[idx] = temp_16_rs1[idx] + temp_16_rs2[idx];
        }
        sADDI16S_vv(temp_16_rd, temp_16_rs1, temp_16_rs2);
        addi16s_vv_cnt += ((temp_16_rd[0] == temp_16_rd_tb[0]) & (temp_16_rd[1] == temp_16_rd_tb[1])) ? 1 : 0;
    }
    printf("[ TEST ] `sADDI8S_vv`    :               %s\n", addi8s_vv_cnt == TB_SIZE ? "Pass" : "Fail");
    printf("[ TEST ] `sADDI16S_vv`   :               %s\n", addi16s_vv_cnt == TB_SIZE ? "Pass" : "Fail");

    return (addi8s_vv_cnt + addi16s_vv_cnt) == (2 * TB_SIZE);
}

bool sADD_vx_TB()
{
    int8_t temp_8_rs1[4], temp_8_rs2, temp_8_rd[4], temp_8_rd_tb[4];
    int16_t temp_16_rs1[2], temp_16_rs2, temp_16_rd[2], temp_16_rd_tb[2];

    int tb_idx;
    // 8-2.1-1.3 : Signed Integer Addition : sADDI8S_vx
    // void sADDI8S_vx(int8_t c[4], int8_t a[4], int8_t b);
    tb_idx = TB_SIZE;
    int addi8s_vx_cnt = 0;
    while (tb_idx--)
    {
        temp_8_rs2 = (rand() % TB_DATA_SIZE_16_BIT);
        int idx = 4;
        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_8_rd_tb[idx] = temp_8_rs1[idx] + temp_8_rs2;
        }
        sADDI8S_vx(temp_8_rd, temp_8_rs1, temp_8_rs2);

        addi8s_vx_cnt += ((temp_8_rd[0] == temp_8_rd_tb[0]) & (temp_8_rd[1] == temp_8_rd_tb[1]) &
                          (temp_8_rd[2] == temp_8_rd_tb[2]) & (temp_8_rd[3] == temp_8_rd_tb[3]))
                             ? 1
                             : 0;
    }
    // 8-2.1-1.4 : Signed Integer Addition : sADDI16S_vx
    // void sADDI16S_vx(int16_t c[2], int16_t a[2], int16_t b);
    tb_idx = TB_SIZE;
    int addi16s_vx_cnt = 0;
    while (tb_idx--)
    {
        temp_16_rs2 = (rand() % TB_DATA_SIZE_16_BIT);
        int idx = 2;
        while (idx--)
        {
            temp_16_rd[idx] = 0;
            temp_16_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rd_tb[idx] = temp_16_rs1[idx] + temp_16_rs2;
        }
        sADDI16S_vx(temp_16_rd, temp_16_rs1, temp_16_rs2);

        addi16s_vx_cnt += ((temp_16_rd[0] == temp_16_rd_tb[0]) & (temp_16_rd[1] == temp_16_rd_tb[1])) ? 1 : 0;
    }
    printf("[ TEST ] `sADDI8S_vx`    :               %s\n", addi8s_vx_cnt == TB_SIZE ? "Pass" : "Fail");
    printf("[ TEST ] `sADDI16S_vx`   :               %s\n", addi16s_vx_cnt == TB_SIZE ? "Pass" : "Fail");

    return (addi8s_vx_cnt + addi16s_vx_cnt) == (2 * TB_SIZE);
}

/********************************************************************************************************************
 *                                        8-2.1-2 : Signed Integer Substration                                      *
 ********************************************************************************************************************/
bool sSUB_vv_TB()
{
    int8_t temp_8_rs1[4], temp_8_rs2[4], temp_8_rd[4], temp_8_rd_tb[4];
    int16_t temp_16_rs1[2], temp_16_rs2[2], temp_16_rd[2], temp_16_rd_tb[2];
    int tb_idx;

    // 8-2.1-2.1 : Signed Integer Substration : sSUBI8S_vv
    // void sSUBI8S_vv(int8_t c[4], int8_t a[4], int8_t b[4]);
    tb_idx = TB_SIZE;
    int subi8s_vv_cnt = 0;
    while (tb_idx--)
    {
        int idx = 4;
        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_8_rs2[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_8_rd_tb[idx] = temp_8_rs1[idx] - temp_8_rs2[idx];
        }
        sSUBI8S_vv(temp_8_rd, temp_8_rs1, temp_8_rs2);
        subi8s_vv_cnt += ((temp_8_rd[0] == temp_8_rd_tb[0]) & (temp_8_rd[1] == temp_8_rd_tb[1]) &
                          (temp_8_rd[2] == temp_8_rd_tb[2]) & (temp_8_rd[3] == temp_8_rd_tb[3]))
                             ? 1
                             : 0;
    }

    // 8-2.1-2.2 : Signed Integer Substration : sSUBI16S_vv
    // void sSUBI16S_vv(int16_t c[2], int16_t a[2], int16_t b[2]);
    tb_idx = TB_SIZE;
    int subi16s_vv_cnt = 0;
    while (tb_idx--)
    {
        int idx = 2;
        while (idx--)
        {
            temp_16_rd[idx] = 0;
            temp_16_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rs2[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rd_tb[idx] = temp_16_rs1[idx] - temp_16_rs2[idx];
        }
        sSUBI16S_vv(temp_16_rd, temp_16_rs1, temp_16_rs2);
        subi16s_vv_cnt += ((temp_16_rd[0] == temp_16_rd_tb[0]) & (temp_16_rd[1] == temp_16_rd_tb[1])) ? 1 : 0;
    }
    printf("[ TEST ] `sSUBI8S_vv`    :               %s\n", subi8s_vv_cnt == TB_SIZE ? "Pass" : "Fail");
    printf("[ TEST ] `sSUBI16S_vv`   :               %s\n", subi16s_vv_cnt == TB_SIZE ? "Pass" : "Fail");

    return (subi8s_vv_cnt + subi16s_vv_cnt) == (2 * TB_SIZE);
}

bool sSUB_vx_TB()
{
    int8_t temp_8_rs1[4], temp_8_rs2, temp_8_rd[4], temp_8_rd_tb[4];
    int16_t temp_16_rs1[2], temp_16_rs2, temp_16_rd[2], temp_16_rd_tb[2];

    int tb_idx;
    // 8-2.1-1.3 : Signed Integer Addition : sADDI8S_vx
    // void sADDI8S_vx(int8_t c[4], int8_t a[4], int8_t b);
    tb_idx = TB_SIZE;
    int subi8s_vx_cnt = 0;
    while (tb_idx--)
    {
        temp_8_rs2 = (rand() % TB_DATA_SIZE_16_BIT);
        int idx = 4;
        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_8_rd_tb[idx] = temp_8_rs1[idx] - temp_8_rs2;
        }
        sSUBI8S_vx(temp_8_rd, temp_8_rs1, temp_8_rs2);

        subi8s_vx_cnt += ((temp_8_rd[0] == temp_8_rd_tb[0]) & (temp_8_rd[1] == temp_8_rd_tb[1]) &
                          (temp_8_rd[2] == temp_8_rd_tb[2]) & (temp_8_rd[3] == temp_8_rd_tb[3]))
                             ? 1
                             : 0;
    }
    // 8-2.1-1.4 : Signed Integer Addition : sADDI16S_vx
    // void sADDI16S_vx(int16_t c[2], int16_t a[2], int16_t b);
    tb_idx = TB_SIZE;
    int subi16s_vx_cnt = 0;
    while (tb_idx--)
    {
        temp_16_rs2 = (rand() % TB_DATA_SIZE_16_BIT);
        int idx = 2;
        while (idx--)
        {
            temp_16_rd[idx] = 0;
            temp_16_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rd_tb[idx] = temp_16_rs1[idx] - temp_16_rs2;
        }
        sSUBI16S_vx(temp_16_rd, temp_16_rs1, temp_16_rs2);
        subi16s_vx_cnt += ((temp_16_rd[0] == temp_16_rd_tb[0]) & (temp_16_rd[1] == temp_16_rd_tb[1])) ? 1 : 0;
    }
    printf("[ TEST ] `sSUBI8S_vx`    :               %s\n", subi8s_vx_cnt == TB_SIZE ? "Pass" : "Fail");
    printf("[ TEST ] `sSUBI16S_vx`   :               %s\n", subi16s_vx_cnt == TB_SIZE ? "Pass" : "Fail");

    return (subi8s_vx_cnt + subi16s_vx_cnt) == (2 * TB_SIZE);
}

/********************************************************************************************************************
 *                                      8-2.1-3 : Signed Integer Multiplication                                     *
 ********************************************************************************************************************/
bool sMUL_vv_TB()
{

    int8_t temp_8_rs1[4], temp_8_rs2[4], temp_8_rd[4], temp_8_rd_tb[4];
    int16_t temp_16_rd[4], temp_16_rd_tb[4];
    int tb_idx;

    // 8-2.1-3.1 : Signed Integer Multiplication : sMULI8I8S_vv
    // void sMULI8I8S_vv(int8_t c[4], int8_t a[4], int8_t b[4]);
    tb_idx = TB_SIZE;
    int muli8i8s_vv_cnt = 0;
    while (tb_idx--)
    {
        int idx = 4;
        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_8_rs2[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            int16_t temp = (int16_t)temp_8_rs1[idx] * (int16_t)temp_8_rs2[idx];
            temp_8_rd_tb[idx] = (int8_t)((temp >> 8) & (int16_t)0x00ff);
        }
        sMULI8I8S_vv(temp_8_rd, temp_8_rs1, temp_8_rs2);
        muli8i8s_vv_cnt += ((temp_8_rd[0] == temp_8_rd_tb[0]) & (temp_8_rd[1] == temp_8_rd_tb[1]) &
                            (temp_8_rd[2] == temp_8_rd_tb[2]) & (temp_8_rd[3] == temp_8_rd_tb[3]))
                               ? 1
                               : 0;
    }
    // 8-2.1-3.7 : Signed Integer Multiplication : sMULI8I16S_vv
    // void sMULI8I16S_vv(int16_t c[4], int8_t a[4], int8_t b[4]);
    tb_idx = TB_SIZE;
    int muli8i16s_vv_HL = 0;
    while (tb_idx--)
    {
        int idx = 4;
        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_8_rs2[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rd_tb[idx] = (int16_t)temp_8_rs1[idx] * (int16_t)temp_8_rs2[idx];
        }
        sMULI8I16S_vv_H(temp_16_rd + 2, temp_8_rs1, temp_8_rs2);
        sMULI8I16S_vv_L(temp_16_rd, temp_8_rs1, temp_8_rs2);
        muli8i16s_vv_HL += ((temp_16_rd[0] == temp_16_rd_tb[0]) & (temp_16_rd[1] == temp_16_rd_tb[1]) &
                            (temp_16_rd[2] == temp_16_rd_tb[2]) & (temp_16_rd[3] == temp_16_rd_tb[3]))
                               ? 1
                               : 0;
    }

    tb_idx = TB_SIZE;
    int muli8i16s_vv_LH = 0;
    while (tb_idx--)
    {
        int idx = 4;
        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_8_rs2[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rd_tb[idx] = (int16_t)temp_8_rs1[idx] * (int16_t)temp_8_rs2[idx];
        }
        sMULI8I16S_vv_L(temp_16_rd, temp_8_rs1, temp_8_rs2);
        sMULI8I16S_vv_H(temp_16_rd + 2, temp_8_rs1, temp_8_rs2);
        muli8i16s_vv_LH += ((temp_16_rd[0] == temp_16_rd_tb[0]) & (temp_16_rd[1] == temp_16_rd_tb[1]) &
                            (temp_16_rd[2] == temp_16_rd_tb[2]) & (temp_16_rd[3] == temp_16_rd_tb[3]))
                               ? 1
                               : 0;
    }

    tb_idx = TB_SIZE;
    int muli8i16s_vv_L = 0;
    while (tb_idx--)
    {
        int idx = 4;
        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_8_rs2[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rd_tb[idx] = (int16_t)temp_8_rs1[idx] * (int16_t)temp_8_rs2[idx];
        }
        sMULI8I16S_vv_L(temp_16_rd, temp_8_rs1, temp_8_rs2);
        muli8i16s_vv_L += ((temp_16_rd[0] == temp_16_rd_tb[0]) & (temp_16_rd[1] == temp_16_rd_tb[1])) ? 1 : 0;
    }

    tb_idx = TB_SIZE;
    int muli8i16s_vv_H = 0;
    while (tb_idx--)
    {
        int idx = 4;
        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_8_rs2[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rd_tb[idx] = (int16_t)temp_8_rs1[idx] * (int16_t)temp_8_rs2[idx];
        }
        sMULI8I16S_vv_H(temp_16_rd + 2, temp_8_rs1, temp_8_rs2);
        muli8i16s_vv_H += ((temp_16_rd[2] == temp_16_rd_tb[2]) & (temp_16_rd[3] == temp_16_rd_tb[3])) ? 1 : 0;
    }
    printf("[ TEST ] `sMULI8I8S_vv`  :               %s\n", muli8i8s_vv_cnt == TB_SIZE ? "Pass" : "Fail");
    printf("[ TEST ] `sMULI8I16S_vv` : .H before .L  %s\n", muli8i16s_vv_HL == TB_SIZE ? "Pass" : "Fail");
    printf("[ TEST ] `sMULI8I16S_vv` : .L before .H  %s\n", muli8i16s_vv_LH == TB_SIZE ? "Pass" : "Fail");
    printf("[ TEST ] `sMULI8I16S_vv` : only .L       %s\n", muli8i16s_vv_L == TB_SIZE ? "Pass" : "Fail");
    printf("[ TEST ] `sMULI8I16S_vv` : only .H       %s\n", muli8i16s_vv_H == TB_SIZE ? "Pass" : "Fail");

    return (muli8i8s_vv_cnt == TB_SIZE) & (muli8i16s_vv_HL == TB_SIZE) & (muli8i16s_vv_LH == TB_SIZE) &
           (muli8i16s_vv_L == TB_SIZE) & (muli8i16s_vv_H == TB_SIZE);
}

bool sMUL_vx_TB()
{
    int8_t temp_8_rs1[4], temp_8_rs2, temp_8_rd[4], temp_8_rd_tb[4];
    int16_t temp_16_rd[4], temp_16_rd_tb[4];
    int tb_idx;

    // 8-2.1-3.4 : Signed Integer Multiplication : sMULI8I8S_vx
    // void sMULI8I8S_vx(int8_t c[4], int8_t a[4], int8_t b);
    tb_idx = TB_SIZE;
    int muli8i8s_vx_cnt = 0;
    while (tb_idx--)
    {
        int idx = 4;
        temp_8_rs2 = (rand() % TB_DATA_SIZE_16_BIT);

        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            int16_t temp = (int16_t)temp_8_rs1[idx] * (int16_t)temp_8_rs2;
            temp_8_rd_tb[idx] = (int8_t)((temp >> 8) & (int16_t)0x00ff);
        }
        sMULI8I8S_vx(temp_8_rd, temp_8_rs1, temp_8_rs2);
        muli8i8s_vx_cnt += ((temp_8_rd[0] == temp_8_rd_tb[0]) & (temp_8_rd[1] == temp_8_rd_tb[1]) &
                            (temp_8_rd[2] == temp_8_rd_tb[2]) & (temp_8_rd[3] == temp_8_rd_tb[3]))
                               ? 1
                               : 0;
    }
    // 8-2.1-3.8 : Signed Integer Multiplication : sMULI8I16S_vx
    // void sMULI8I16S_vx(int16_t c[2], int8_t a[4], int8_t b);
    tb_idx = TB_SIZE;
    int muli8i16s_vx_HL = 0;
    while (tb_idx--)
    {
        int idx = 4;
        temp_8_rs2 = (rand() % TB_DATA_SIZE_16_BIT);

        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rd_tb[idx] = (int16_t)temp_8_rs1[idx] * (int16_t)temp_8_rs2;
        }
        sMULI8I16S_vx_H(temp_16_rd + 2, temp_8_rs1, temp_8_rs2);
        sMULI8I16S_vx_L(temp_16_rd, temp_8_rs1, temp_8_rs2);
        muli8i16s_vx_HL += ((temp_16_rd[0] == temp_16_rd_tb[0]) & (temp_16_rd[1] == temp_16_rd_tb[1]) &
                            (temp_16_rd[2] == temp_16_rd_tb[2]) & (temp_16_rd[3] == temp_16_rd_tb[3]))
                               ? 1
                               : 0;
    }

    tb_idx = TB_SIZE;
    int muli8i16s_vx_LH = 0;
    while (tb_idx--)
    {
        int idx = 4;
        temp_8_rs2 = (rand() % TB_DATA_SIZE_16_BIT);

        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rd_tb[idx] = (int16_t)temp_8_rs1[idx] * (int16_t)temp_8_rs2;
        }
        sMULI8I16S_vx_L(temp_16_rd, temp_8_rs1, temp_8_rs2);
        sMULI8I16S_vx_H(temp_16_rd + 2, temp_8_rs1, temp_8_rs2);
        muli8i16s_vx_LH += ((temp_16_rd[0] == temp_16_rd_tb[0]) & (temp_16_rd[1] == temp_16_rd_tb[1]) &
                            (temp_16_rd[2] == temp_16_rd_tb[2]) & (temp_16_rd[3] == temp_16_rd_tb[3]))
                               ? 1
                               : 0;
    }

    tb_idx = TB_SIZE;
    int muli8i16s_vx_L = 0;
    while (tb_idx--)
    {
        int idx = 4;
        temp_8_rs2 = (rand() % TB_DATA_SIZE_16_BIT);

        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rd_tb[idx] = (int16_t)temp_8_rs1[idx] * (int16_t)temp_8_rs2;
        }
        sMULI8I16S_vx_L(temp_16_rd, temp_8_rs1, temp_8_rs2);
        muli8i16s_vx_L += ((temp_16_rd[0] == temp_16_rd_tb[0]) & (temp_16_rd[1] == temp_16_rd_tb[1])) ? 1 : 0;
    }

    tb_idx = TB_SIZE;
    int muli8i16s_vx_H = 0;
    while (tb_idx--)
    {
        int idx = 4;
        temp_8_rs2 = (rand() % TB_DATA_SIZE_16_BIT);

        while (idx--)
        {
            temp_8_rd[idx] = 0;
            temp_8_rs1[idx] = (rand() % TB_DATA_SIZE_16_BIT);
            temp_16_rd_tb[idx] = (int16_t)temp_8_rs1[idx] * (int16_t)temp_8_rs2;
        }
        sMULI8I16S_vx_H(temp_16_rd + 2, temp_8_rs1, temp_8_rs2);
        muli8i16s_vx_H += ((temp_16_rd[2] == temp_16_rd_tb[2]) & (temp_16_rd[3] == temp_16_rd_tb[3])) ? 1 : 0;
    }

    printf("[ TEST ] `sMULI8I8S_vx`  :               %s\n", muli8i8s_vx_cnt == TB_SIZE ? "Pass" : "Fail");
    printf("[ TEST ] `sMULI8I16S_vx` : .H before .L  %s\n", muli8i16s_vx_HL == TB_SIZE ? "Pass" : "Fail");
    printf("[ TEST ] `sMULI8I16S_vx` : .L before .H  %s\n", muli8i16s_vx_LH == TB_SIZE ? "Pass" : "Fail");
    printf("[ TEST ] `sMULI8I16S_vx` : only .L       %s\n", muli8i16s_vx_L == TB_SIZE ? "Pass" : "Fail");
    printf("[ TEST ] `sMULI8I16S_vx` : only .H       %s\n", muli8i16s_vx_H == TB_SIZE ? "Pass" : "Fail");

    return (muli8i8s_vx_cnt == TB_SIZE) & (muli8i16s_vx_HL == TB_SIZE) & (muli8i16s_vx_LH == TB_SIZE) &
           (muli8i16s_vx_L == TB_SIZE) & (muli8i16s_vx_H == TB_SIZE);
    return true;
}
