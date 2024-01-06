/*
 * Copyright 2021 The CFU-Playground Authors
 * Copyright (c) 2023 Playlab, National Cheng Kung University.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "proj_menu.h"

#include <stdio.h>

#include "acal_lab/tb/includes/op.h"
#include "acal_lab/tb/includes/simd.h"
#include "cfu.h"
#include "menu.h"

using namespace acal_lab;

namespace
{

void do_SIMD_TB_lab(void)
{
    printf("=============== SIMD Vector-Vector ================\n");
    bool vvTB = tb::tb_sADDS_vv() & tb::tb_sSUBS_vv() & tb::tb_sPMULI8I16S_vv() & tb::tb_sAMULI8I8S_vv_NQ();
    printf("===================================================\n");
    printf("SIMD Integer Extension : (Lab Edition)         %s\n", vvTB ? "Pass" : "Fail");
    printf("===================================================\n");
}

void do_SIMD_TB_hw(void)
{
    printf("=============== SIMD Vector-Vector ================\n");
    bool vvTB = tb::tb_sADDS_vv() & tb::tb_sSUBS_vv() & tb::tb_sPMULI8I16S_vv() & tb::tb_sAMULI8I8S_vv();
    printf("=> SUMMARY | SIMD Vector-Vector :              %s\n", vvTB ? "Pass" : "Fail");
    printf("=============== SIMD Vector-Scalar ================\n");
    bool vxTB = tb::tb_sADDS_vx() & tb::tb_sSUBS_vx() & tb::tb_sPMULI8I16S_vx() & tb::tb_sAMULI8I8S_vx();
    printf("=> SUMMARY | SIMD Vector-Scalar :              %s\n", vxTB ? "Pass" : "Fail");
    printf("===================================================\n");
    printf("SIMD Integer Extension : (Homework Edition)    %s\n", vvTB & vxTB ? "Pass" : "Fail");
    printf("===================================================\n");
}

void do_GEMM_with_SIMD_lab(void)
{
    printf("==============   GEMM with SIMD   =================\n");
    printf("\n`GEMM` with SIMD (LAB Edition)            : %s\n", tb::tb_Gemm(tb::testType::LAB) ? "Pass" : "Fail");
    printf("===================================================\n");
}

void do_GEMM_with_SIMD_hw(void)
{
    printf("==============   GEMM with SIMD   =================\n");
    printf("\n`GEMM` with SIMD (Homework Edition)       : %s\n", tb::tb_Gemm(tb::testType::HW) ? "Pass" : "Fail");
    printf("===================================================\n");
}

// void do_CONV_with_SIMD(void)
// {
//     printf("%s\n", myConv_Kernel() ? "Correct" : "Wrong");
// }

// void do_RELU_with_SIMD(void)
// {
//     printf("%s\n", myReLU_Kernel() ? "Correct" : "Wrong");
// }

// void do_MXPL_with_SIMD(void)
// {
//     printf("%s\n", myMxPl_Kernel() ? "Correct" : "Wrong");
// }

// void do_OP_with_SIMD(void)
// {
//     printf("%s\n", myGemm_Kernel() & myConv_Kernel() & myReLU_Kernel() &
//     myMxPl_Kernel() ? "Correct" : "Wrong");
// }

// void do_ALEXNET_INFERENCE(void)
// {
//     myAlexNetTB();
// }

struct Menu MENU = {
    "Project Menu",
    "project",
    {
        MENU_ITEM('0', "test SIMD inst      - Integer Extension (Lab Edition)", do_SIMD_TB_lab),
        MENU_ITEM('1', "test SIMD inst      - Integer Extension (Homework Edidtion)", do_SIMD_TB_hw),
        MENU_ITEM('2', "test GEMM Operation - with SIMD Extension (Lab Edition)", do_GEMM_with_SIMD_lab),
        MENU_ITEM('3', "test GEMM Operation - with SIMD Extension (Homework Edidtion)", do_GEMM_with_SIMD_hw),
        // MENU_ITEM('c', "test CONV Operation - with SIMD Extension",
        // do_CONV_with_SIMD), MENU_ITEM('m', "test MXPL Operation - with SIMD
        // Extension", do_MXPL_with_SIMD), MENU_ITEM('r', "test RELU Operation -
        // with SIMD Extension", do_RELU_with_SIMD), MENU_ITEM('o', "test ALL
        // Operation - with SIMD Extension", do_OP_with_SIMD), MENU_ITEM('a',
        // "test AlexNet        - with SIMD Extension", do_ALEXNET_INFERENCE),
        MENU_END,
    },
};

}; // anonymous namespace

extern "C" void do_proj_menu()
{
    menu_run(&MENU);
}
