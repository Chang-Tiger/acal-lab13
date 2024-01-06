#ifndef _TB_OP_GEMM_H_
#define _TB_OP_GEMM_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "acal_lab/includes/op/Gemm.h"
#include "tb_Op.h"

#define GEMM_dimM 8
#define GEMM_dimN 8
#define GEMM_dimK 8
#define _8_BIT_ 256

namespace acal_lab
{
namespace tb
{
class TB_Gemm : public tb_Operator
{
  protected:
    gemmInfo *info;

  public:
    TB_Gemm(tensorInfo *opt, tensorInfo *ipt, gemmInfo *opInfo, quantiInfo *quantInfo, QauntiType quantType)
        : tb_Operator(opt, ipt, quantInfo)
    {
        this->info = opInfo;
        switch (quantType)
        {
        case PER_LAYER_NAIVE_QUANTI:
            execFunction = reinterpret_cast<void (tb_Operator::*)()>(&TB_Gemm::execPerLayerNaiveQuant);
            break;
        case PER_LAYER_ADVANCE_QUANTI:
            execFunction = reinterpret_cast<void (tb_Operator::*)()>(&TB_Gemm::execPerLayerAdvanceQuant);
            break;
        case PER_OPERATION_NAIVE_QUANTI:
            execFunction = reinterpret_cast<void (tb_Operator::*)()>(&TB_Gemm::execPerOperationNaiveQuant);
            break;
        case PER_OPERATION_ADVANCE_QUANTI:
            execFunction = reinterpret_cast<void (tb_Operator::*)()>(&TB_Gemm::execPerOperationAdvanceQuant);
            break;
        }
    }
    void execPerLayerNaiveQuant();
    void execPerLayerAdvanceQuant();
    void execPerOperationNaiveQuant();
    void execPerOperationAdvanceQuant();
};

bool tb_Gemm(testType);
bool tb_GemmPerOperationrNaiveQuant(int8_t *, int8_t *, int8_t *, int8_t *, int8_t *);
bool tb_GemmPerOperationAdvanceQuant(int8_t *, int8_t *, int8_t *, int8_t *, int8_t *);
bool tb_GemmPerLayerNaiveQuant(int8_t *, int8_t *, int16_t *, int16_t *, int16_t *);
bool tb_GemmPerLayerAdvanceQuant(int8_t *, int8_t *, int16_t *, int16_t *, int16_t *);
void randomInitGemm8(int8_t *, int8_t *, int8_t *);
void randomInitGemm16(int8_t *, int8_t *, int16_t *);
bool compare8(int8_t *, int8_t *);
bool compare16(int16_t *, int16_t *);

}; // namespace tb
}; // namespace acal_lab
#endif // _TB_OP_GEMM_H_