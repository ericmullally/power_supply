/*
 * systick.h
 *
 *  Created on: Feb 3, 2023
 *      Author: eric
 */

#ifndef SYSTIC_H_
#define SYSTIC_H_

#include <stm32f4xx.h>

void Delay(int ms);

void systick_1hz_interupt(void);

#endif /* SYSTIC_H_ */
