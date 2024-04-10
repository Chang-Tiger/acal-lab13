#include "acal_lab/includes/op/simd/Conv.h"
namespace acal_lab {
namespace simd {

void Conv::execPerLayerNaiveQuant() {
	/********************************************************************
	 * TODO:                                                            *
	 * For Homework 8.4, your task is to implement CONV with per Layer  *
	 * `Naive Quantization`. This involves using the instruction        *
	 * `sPMULI8I16S(.vv/.vx)` to generate int16 output. However, the    *
	 * int16 output needs to be converted to int8 and then forwarded    *
	 * to the next Operator. To achieve this, utilize `sQNTI16I8S.vv.NQ`*
	 * for the conversion.                                              *
	 *******************************************************************/
}

void Conv::execPerLayerAdvanceQuant() {
	/***********************************************************
	 * TODO:                                                   *
	 * For Homework 8.4, implement CONV with per Operation     *
	 * `Advance Quantization`. Update instruction in the       *
	 * `void acal_lab::Conv::execPerLayerNaiveQuant()`         *
	 * function from `sQNTI16I8S.vv.NQ` to `sQNTI16I8S.vv.AQ`. *
	 **********************************************************/
}

void Conv::execPerOperationNaiveQuant() {
	/********************************************************
	 * TODO:                                                *
	 * For Homework 8.3, implement CONV with per Operation  *
	 * Naive Quantization. Utilize `sAMULI8I8S(.vv/.vx).NQ` *
	 * to generate int8 output.                             *
	 *******************************************************/
}

void Conv::execPerOperationAdvanceQuant() {
	/********************************************************
	 * TODO:                                                *
	 * For Homework 8.4, implement CONV with per Operation  *
	 * Advance Quantization. Update instruction in the      *
	 * `void acal_lab::Conv::execPerOperationNaiveQuant()`  *
	 * function from `sAMULI8I8S(.vv/.vx).NQ` to            *
	 * `sAMULI8I8S(.vv/.vx).AQ`.                            *
	 *******************************************************/
}

}  // namespace simd
}  // namespace acal_lab
