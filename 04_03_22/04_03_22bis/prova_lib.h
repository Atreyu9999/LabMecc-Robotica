/*
 * prova_lib.h
 *
 *  Created on: Mar 4, 2022
 *      Author: andre
 */

#ifndef INC_PROVA_LIB_H_
#define INC_PROVA_LIB_H_

#include "main.h"

struct my_struct {
	uint8_t my_unsigned_int_var;
	int8_t s_my_signed_int_var;
	float s_my_float_array[3];
};
void sum_my_array(float *input_float, uint8_t array_dim);
// input float pointer, in reality we have a three elements float array
// given the first two elements of the array, the third element is the sum of them




#endif /* INC_PROVA_LIB_H_ */
