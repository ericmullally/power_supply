/*
 * VIM_878.h
 *
 *  Created on: Mar 23, 2023
 *      Author: Eric
 */

#ifndef VIM_878_H_
#define VIM_878_H_

#include <stm32f401xc.h>
#include <stdint.h>
#include <stdio.h>



/// Addresses in DDRAM. Each Pin occupies 4 bits and are sequentially tied to the next pin
/// Each segment has 16 bits total. EX: segment one is controlled by PINs 1,2,35,36.
/// pin1 is the first 4 bits pin 2 is the second 4 bits in 0x00.
#define PIN1 	0x00
#define PIN3 	0x03
#define PIN5 	0x05
#define PIN7 	0x07
#define PIN9 	0x09
#define PIN11 	0x0B
#define PIN13 	0x0D
#define PIN15 	0x0F
#define PIN21	0x10
#define PIN23	0x14
#define PIN25	0x19
#define PIN27	0x1B
#define PIN29	0x1D
#define PIN31	0x1F
#define PIN33	0x21
#define PIN35	0x23


struct SEGMENT{
	char firstPinAddress;
	char secondPinAddress;
};


//
const char INTEGERS[9][2] = {
		{0xE0, 0x70},
		{0x00, 0x60},
		{0xC4, 0x32}
};


















#endif /* VIM_878_H_ */
