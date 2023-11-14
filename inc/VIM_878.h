/*
 * VIM_878.h
 *
 *  Created on: Mar 23, 2023
 *      Author: Eric
 */

#ifndef VIM_878_H_
#define VIM_878_H_

#include <stm32f4xx.h>
#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "Letter_hash.h"
//#include <systick.h>
//#include <I2C.h>
//#include <Letter_hash.h>

/// Addresses in DDRAM. Each Pin occupies 4 bits and are sequentially tied to the next pin
/// Each segment has 16 bits total. EX: segment one is controlled by PINs 1,2,35,36.
/// pin1 is the first 4 bits pin 2 is the second 4 bits in 0x00.
#define PIN0 				0x00
#define PIN2 				0x02
#define PIN4 				0x04
#define PIN6 				0x06
#define PIN8 				0x08
#define PIN10 				0x0A
#define PIN12 				0x0C
#define PIN14 				0x0E

#define PIN21				0x10
#define PIN23				0x12
#define PIN25				0x14
#define PIN27				0x16
#define PIN29				0x18
#define PIN31				0x1A
#define PIN33				0x1C
#define PIN35				0x1E

#define DECIMAL 			0x80



#define ICSET				0x6A // 0110 1010
#define DISPLAY_CONTROL 	0x22
#define BINK_CTRL			0x70
#define ALL_PL_CTRL 		0x7C
#define MODE_SET 			0x48

#define SLAVE_ADDRESS_BSE   0x3E //0011 1110
#define COMMAND				0x80
#define DATA				~COMMAND

#define DISPLAY_OFF 		(MODE_SET &~(0x08))
#define DISPLAY_ON		 	 MODE_SET


struct SEGMENT{
	char firstPinAddress;
	char secondPinAddress;
};




void lcd_init(I2C_HandleTypeDef hI2Cx);
void display_Write(I2C_HandleTypeDef hI2Cx, int segment, int num, int decimal);
void burst_write(I2C_HandleTypeDef hI2Cx, int startSeg, int length, int * nums);
void clearDisplay(I2C_HandleTypeDef hI2Cx);
#endif /* VIM_878_H_ */
