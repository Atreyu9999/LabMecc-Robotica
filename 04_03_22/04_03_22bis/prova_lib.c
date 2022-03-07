/*
 * prova_lib.c
 *
 *  Created on: Mar 4, 2022
 *      Author: andre
 */


#include "prova_lib.h"
void sum_my_array(float *input_float, uint8_t array_dim) {
	input_float[array_dim -1] = input_float[array_dim-2] = input_float[array_dim-3];
}
