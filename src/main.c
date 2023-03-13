

#include <stdint.h>
#include <stdio.h>
#include <lcd.h>
#include <Usart_Setup.h>
#include <adc.h>



enum chars{
	A = 0x41, B = 0x42, C = 0x43, D = 0x44, E = 0x45, F = 0x46, G = 0x47, H = 0x48,
	I = 0x49, J = 0x4A, K = 0x4B, L = 0x4C, M = 0x4D, N = 0x4E, O = 0x4F, P = 0x50, Q = 0x51,
	R = 0x52, S = 0x53, T = 0x54, U = 0x55, V = 0x56, W = 0x57, X = 0x58, Y = 0x59, Z = 0x5A
}myCharacters;


// lcd code for nums 0-9. could also just add 0x30 to the desired value
uint8_t numbers[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

typedef struct {
	int whole;
	int tenths;
	int hundredths;
}voltageNums;


voltageNums pull_float(float val);



int main(void)
{
	// Enable FPU
	SCB->CPACR |= ((3UL << 20)|(3UL << 22));

	usart2_Tx_init();
	adc_init();
	float converted = 0;

    /* Loop forever */
	while(1){
		start_single_conversion();

		/* Get the read out of adc as a float*/
		converted = read_conversion();

		/* Change the float into whole numbers*/
		voltageNums final = pull_float(converted);

		/* Store the whole numbers as the code required by the dog lcd.*/
		uint8_t whole = numbers[final.whole];
		uint8_t tenths = numbers[final.tenths];
		uint8_t hundreths = numbers[final.hundredths];
		printf("%d, %d, %d, %c \n\r", whole, tenths, hundreths, V );
		Delay(500);
	}
}

/*@brief Changes a floating point number into 3 whole numbers
 * */
voltageNums pull_float(float val){
	voltageNums myNums;
	myNums.whole = (int)val;
	myNums.tenths = ((int)(val * 10)%(int)val);
	myNums.hundredths = ((int)(val * 100)%(int)(val * 10));
	return myNums;
}
