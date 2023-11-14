/*
 * I2C.h
 *
 *  Created on: Oct 25, 2023
 *      Author: eric
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f401xc.h"
#include <stdio.h>


#define I2C_FM_MODE			1
#define I2C_SM_MODE			0

#define I2C1_ALT_PINS 		1   // I2C1 Default pins are (PB6-7) ALT Pins are (PB8-9)
#define I2C1_DEFAULT_PINS 	0



typedef struct{
uint8_t Own_Address; // only for slave mode
uint8_t ACK_Control;
uint8_t No_Stretch;
uint8_t Mode;
uint8_t Pin_Choice_default; // only effective if using I2C1
}I2C_Config_T;


typedef struct {
	I2C_TypeDef * I2Cx;
	I2C_Config_T config;
}I2C_Instance_T;

void I2C_Burst_Write(I2C_TypeDef * tI2Cx, uint8_t slave_bse_add, uint8_t* msg, size_t size );
void I2C_Init(I2C_Instance_T * pI2Cx);

#endif /* I2C_H_ */
// SM < 100Khz
// FM < 400Khz
// SM mode CCR  T_high and T_LOW = CCR*T_PCLK1
// T_PCLCK = Peripheral clock FREQ

/*If DUTY =0
 * 	T_High = CCR * T_PCLK1
 * 	T_Low = 2 * CCR * T_PCLK1
 *If DUTY = 1
 * 	T_High = 9 * CCR * T_PCLK1
 * 	T_Low = 16 * CCR * T_PCLK1
 * */


// 0010 1000
// 16mhz = T_PCLCK 62.5ns


// T_High = T_r(SCL) + T_w(SCLH)
// T_High = 1000 + 4000 = 5000


// T_low = T_f(SCL) + T_w(SCLL)
// T_low = 300 + 4700 = 5000
// CCR = 80d = 0x60


// SM mode CCR  T_high and T_LOW = CCR*T_PCLK1

