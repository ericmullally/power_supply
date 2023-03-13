/*
 * adc.h
 *
 *  Created on: Mar 10, 2023
 *      Author: eric
 */

#ifndef ADC_H_
#define ADC_H_

#include <stm32f4xx.h>


void adc_init(void);
void start_single_conversion(void);
double read_conversion(void);
#endif /* ADC_H_ */
