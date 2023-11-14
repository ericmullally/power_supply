/*
 * VIM_878.c
 *
 *  Created on: Mar 23, 2023
 *      Author: Eric
 */
#include <VIM_878.h>


typedef struct{
	volatile char firstCommands[3];
	volatile char secondCommands[3];
}numberDisCmnds;



numberDisCmnds buildDisplayComand(int segment, char topVal, char btmVal );
int hash(char letter);

struct SEGMENT segment1;
struct SEGMENT segment2;
struct SEGMENT segment3;
struct SEGMENT segment4;
struct SEGMENT segment5;
struct SEGMENT segment6;
struct SEGMENT segment7;
struct SEGMENT segment8;


const char INTEGERS[10][2] = {
		{0xE0, 0x70},
		{0x00, 0x60},
		{0xC4, 0x32},
		{0x84, 0x72},
		{0x24, 0x62},
		{0xA4, 0x52},
		{0xE4, 0x52},
		{0x00, 0x70},
		{0xE4, 0x72},
		{0xA4, 0x72}
};

char alphabet[26];


void load_segments(void){

	segment1.firstPinAddress = PIN0;
	segment1.secondPinAddress = PIN35;

	segment2.firstPinAddress = PIN2;
	segment2.secondPinAddress = PIN33;

	segment3.firstPinAddress = PIN4;
	segment3.secondPinAddress = PIN31;

	segment4.firstPinAddress = PIN6;
	segment4.secondPinAddress = PIN29;

	segment5.firstPinAddress = PIN8;
	segment5.secondPinAddress = PIN27;

	segment6.firstPinAddress = PIN10;
	segment6.secondPinAddress = PIN25;

	segment7.firstPinAddress = PIN12;
	segment7.secondPinAddress = PIN23;

	segment8.firstPinAddress = PIN14;
	segment8.secondPinAddress = PIN21;

}

void lcd_init(I2C_HandleTypeDef hI2Cx){

	load_segments();
	letter_hash_init();
	volatile char commands[6] = {
			(ICSET | COMMAND),
			(DISPLAY_OFF | COMMAND),
			(DISPLAY_CONTROL | COMMAND),
			(BINK_CTRL | COMMAND),
			(ALL_PL_CTRL | COMMAND),
			(DISPLAY_ON | COMMAND)
	};

	HAL_Delay(1);
	HAL_I2C_Master_Transmit(&hI2Cx, (SLAVE_ADDRESS_BSE << 1), (uint8_t *)commands, sizeof(commands), 100000);
	HAL_Delay(1);
	clearDisplay(hI2Cx);


}

/*
 * @brief Clear the LCD
 *
 * Clears all segments of the display.
 * */
void clearDisplay(I2C_HandleTypeDef hI2Cx){
	volatile char commands[18] = {
			(DISPLAY_ON | COMMAND),
			(segment1.firstPinAddress),
			(0x00), //1,2
			(0x00), //3,4
			(0x00),	//5,6
			(0x00), //7,8
			(0x00),	//9,10
			(0x00),	//11,12
			(0x00),	//13,14
			(0x00),	//15,16
			(0x00), //21,22
			(0x00),	//23,24
			(0x00), //25,26
			(0x00), //27,28
			(0x00), //29,30
			(0x00), //31,32
			(0x00), //33,34
			(0x00), //35,36
	};
	HAL_Delay(1);
	HAL_I2C_Master_Transmit(&hI2Cx, (SLAVE_ADDRESS_BSE << 1), (uint8_t *)commands, sizeof(commands), 100000);
}

/*@brief Builds the needed commands to display a number.
 *
 *
 * Finds the addres of the requested segment and bundles the into two seperate arrays
 * stored in the numberDisCmnds struct. Then returns that package.
 *
 * @param int segment, char topVal, char btmVal
 *
 * @return numberDisCmnds
 * */
numberDisCmnds buildDisplayComand(int segment, char topVal, char btmVal ){

	numberDisCmnds computed;
	computed.firstCommands[0]= (DISPLAY_ON | COMMAND);
	computed.secondCommands[0] = (DISPLAY_ON | COMMAND);

	switch(segment){
	case 1:
		computed.firstCommands[1] = segment1.firstPinAddress;
		computed.secondCommands[1] = segment1.secondPinAddress;
		break;
	case 2:
		computed.firstCommands[1] = segment2.firstPinAddress;
		computed.secondCommands[1] = segment2.secondPinAddress;
		break;
	case 3:
		computed.firstCommands[1] = segment3.firstPinAddress;
		computed.secondCommands[1] = segment3.secondPinAddress;
		break;
	case 4:
		computed.firstCommands[1] = segment4.firstPinAddress;
		computed.secondCommands[1] = segment4.secondPinAddress;
		break;
	case 5:
		computed.firstCommands[1] = segment5.firstPinAddress;
		computed.secondCommands[1] = segment5.secondPinAddress;
		break;
	case 6:
		computed.firstCommands[1] = segment6.firstPinAddress;
		computed.secondCommands[1] = segment6.secondPinAddress;
		break;
	case 7:
		computed.firstCommands[1] = segment7.firstPinAddress;
		computed.secondCommands[1] = segment7.secondPinAddress;
		break;
	case 8:
		computed.firstCommands[1] = segment8.firstPinAddress;
		computed.secondCommands[1] = segment8.secondPinAddress;
		break;
	default:
		break;
	}

	computed.firstCommands[2] = topVal;
	computed.secondCommands[2] = btmVal;

	return computed;
}

/*
 * @brief  Writes a single number to a specific segment.
 *
 *
 *
 * Takes in a segment to write to and the number to write.
 * then looks the code for the number in the lookup table.
 * Feeds those codes into the buildDisplay function to build out the I2C commands.
 * Then sends the commands to the LCD.
 *
 * @param int Segment number, int number to be displayed.
 * */
void display_Write(I2C_HandleTypeDef hI2Cx, int segment, int num, int decimal){
	numberDisCmnds commands_all;

	char val_T;
	char val_B;
	if(segment == 8){
		struct Letter * values = get_letter("V");
		val_T = values->top;
		val_B = values->bottom;
	}else if(segment == 7){
		val_T = 0x00;
		val_B = 0x00;
	}else{
		val_T = INTEGERS[num][0];
		val_B = INTEGERS[num][1];
	}


	commands_all = buildDisplayComand(segment, val_T, val_B);

	if(decimal){
		commands_all.secondCommands[2] |= DECIMAL;
	}

	HAL_I2C_Master_Transmit(&hI2Cx, (SLAVE_ADDRESS_BSE << 1), (uint8_t *)commands_all.firstCommands, 3, 100000);
	HAL_Delay(1);
	HAL_I2C_Master_Transmit(&hI2Cx, (SLAVE_ADDRESS_BSE << 1), (uint8_t *)commands_all.secondCommands, 3, 100000);
	HAL_Delay(1);
}


void burst_write(I2C_HandleTypeDef hI2Cx, int startSeg, int length, int * nums){
	clearDisplay(hI2Cx);
	HAL_Delay(1);

	int segment = startSeg;
	for(int i = 0; i < length; i++){
		if(segment == 4){
			display_Write(hI2Cx, segment, *nums++, 1);
		}else{
			display_Write(hI2Cx, segment, *nums++, 0);
		}

		segment ++;
	}

}













