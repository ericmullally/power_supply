/*
 * VIM_878.c
 *
 *  Created on: Mar 23, 2023
 *      Author: Eric
 */
#include <VIM_878.h>

struct SEGMENT segment1;
struct SEGMENT segment2;
struct SEGMENT segment3;
struct SEGMENT segment4;
struct SEGMENT segment5;
struct SEGMENT segment6;
struct SEGMENT segment7;
struct SEGMENT segment8;

void load_segments(void){

	segment1.firstPinAddress = PIN1;
	segment1.secondPinAddress = PIN35;

	segment2.firstPinAddress = PIN3;
	segment2.secondPinAddress = PIN33;

	segment3.firstPinAddress = PIN5;
	segment3.secondPinAddress = PIN31;

	segment4.firstPinAddress = PIN7;
	segment4.secondPinAddress = PIN29;

	segment5.firstPinAddress = PIN9;
	segment5.secondPinAddress = PIN27;

	segment6.firstPinAddress = PIN11;
	segment6.secondPinAddress = PIN25;

	segment7.firstPinAddress = PIN13;
	segment7.secondPinAddress = PIN23;

	segment8.firstPinAddress = PIN15;
	segment8.secondPinAddress = PIN21;

}

void lcd_init(void){

	load_segments();

	I2C1->CR1 |= I2C_STOP_FLAG;

	char commands[21] = {
			(DISPLAY_OFF |= COMMAND),
			(segment1.firstPinAddress),

			(integers[8][0]), // top segment 1
			(integers[8][0]), // top segment 2
			(integers[8][0]), // top segment 3
			(integers[8][0]), // top segment 4
			(integers[8][0]), // top segment 5
			(integers[8][0]), // top segment 6
			(integers[8][0]), // top segment 7
			(integers[8][0]), // top segment 8

			(integers[8][1]), // BTM segment 8
			(integers[8][1]), // BTM segment 7
			(integers[8][1]), // BTM segment 6
			(integers[8][1]), // BTM segment 5
			(integers[8][1]), // BTM segment 4
			(integers[8][1]), // BTM segment 3
			(integers[8][1]), // BTM segment 2
			(integers[8][1]), // BTM segment 1
	};
	Delay(1);
	i2c_burst_write(SLAVE_ADDRESS_BSE, (ICSET |= COMMAND), sizeof(commands), commands);
}

void lcd_write(int whole, int tenths, int hundredths){

	I2C1->CR1 |= I2C_STOP_FLAG;

	char commands[21] = {
			(DISPLAY_CONTROL |= COMMAND),
			(BINK_CTRL |= COMMAND),
			(ALL_PL_CTRL |= COMMAND),
			(DISPLAY_ON |= COMMAND),
			(segment1.firstPinAddress),

			(0x00), // top segment 1
			(0x00), // top segment 2
			(0x00), // top segment 3
			(0x00), // top segment 4
			(integers[whole][0] |= DECIMAL), // top segment 5
			(integers[tenths][0]), 			 // top segment 6
			(integers[hundredths][0]), 		 // top segment 7
			(integers[8][0]), 				 // top segment 8

			(integers[8][1]), 			// BTM segment 8
			(integers[hundredths][1]),	// BTM segment 7
			(integers[tenths][1]), 		// BTM segment 6
			(integers[whole][1]), 		// BTM segment 5
			(0x00), // BTM segment 4
			(0x00), // BTM segment 3
			(0x00), // BTM segment 2
			(0x00), // BTM segment 1
	};
	Delay(1);
	i2c_burst_write(SLAVE_ADDRESS_BSE, (ICSET |= COMMAND), sizeof(commands), commands);

}

