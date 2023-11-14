/*
 * I2C.h
 *
 *  Created on: Oct 25, 2023
 *      Author: eric
 */


#include "I2C.h"

// CR1 Registers
#define I2C_PE_BIT_POS 			0
#define I2C_SMBUS_BIT_POS		1
#define I2C_SMBTYPE_BIT_POS		3
#define I2C_ENARP_BIT_POS		4
#define I2C_ENPEC_BIT_POS		5
#define I2C_ENGC_BIT_POS		6
#define I2C_NOSTRETCH_BIT_POS 	7
#define I2C_START_BIT_POS		8
#define I2C_STOP_BIT_POS		9
#define I2C_ACK_BIT_POS			10
#define I2C_POS_BIT_POS			11
#define I2C_PEC_BIT_POS			12
#define I2C_ALERT_BIT_POS		13
#define I2C_SWRST_BIT_POS		15

// CR2 Registers
#define I2C_FREQ_BIT_POS		0
#define I2C_ITRREN_BIT_POS		8
#define I2C_ITEVTEN_BIT_POS		9
#define I2C_ITBUFEN_BIT_POS		10
#define I2C_DMAEN_BIT_POS		11

// OAR1 Registers
#define I2C_ADD0_BIT_POS		0
#define I2C_ADD7_BIT_POS		1
#define I2C_ADDMODE_BIT_POS		15

// Status Register 1
#define I2C_SB_BIT_POS			0
#define I2C_ADDR_BIT_POS		1
#define I2C_BTF_BIT_POS			2
#define I2C_ADD10_BIT_POS		3
#define I2C_STOPF_BIT_POS		4
#define I2C_RXNE_BIT_POS		6
#define I2C_TXE_BIT_POS			7
#define I2C_BERR_BIT_POS		8
#define I2C_ARLO_BIT_POS		9
#define I2C_AF_BIT_POS			10
#define I2C_OVR_BIT_POS			11
#define I2C_PECERR_BIT_POS		12
#define I2C_TIMEOUT_BIT_POS		14
#define I2C_SMBALERT_BIT_POS	15

// Status register 2
#define I2C_BUSY_BIT_POS		1

// Clock control register
#define I2C_CCR_BIT_POS			0
#define I2C_CCR_DUTY_BIT_POS	14
#define I2C_CCR_FS_BIT_POS		15

int SDA_PIN;
int SCL_PIN;

// Clock enable
#define I2C1_PCLCK_EN()			(RCC->APB1ENR |= (1UL << 21))
#define I2C2_PCLCK_EN()			(RCC->APB1ENR |= (1UL << 22))

// Private functions
uint32_t Get_APB1_ClockSpeed();
void Gen_Start_condition(I2C_TypeDef * tI2Cx);
void Gen_Stop_condition(I2C_TypeDef * tI2Cx);

// Function definitions
void I2C_Gpio_Init(I2C_Instance_T * pI2Cx){

	// Enable Port B clock
	RCC->AHB1ENR |= (1UL << 2);

	if(pI2Cx->I2Cx == I2C1){
		if(pI2Cx->config.Pin_Choice_default == I2C1_ALT_PINS){
			// Pins are PB8-9
			SCL_PIN = 8;
			SDA_PIN = 9;

			// Set Pin mode to Alternate function
			GPIOB->MODER &= ~((3UL << 16) |  (3UL << 18));
			GPIOB->MODER |=  ((2UL << 16) |  (2UL << 18));

			// Set the alternate function to AF04
			GPIOB->AFR[1] &= ~((15UL << 0)|(15UL << 4));
			GPIOB->AFR[1] |=  ((4UL << 0)|(4UL << 4));

			// Set open Drain
			GPIOB->OTYPER |= ((1UL << 8)| (1UL << 9));

			// Set internal pull up
			GPIOB->PUPDR &= ~((3UL << 16) |  (3UL << 18));
			GPIOB->PUPDR |=  ((1UL << 16) |  (1UL << 18));

		}else{
			// Pins are PB6-7
			SCL_PIN = 6;
			SDA_PIN = 7;
			// Set Pin mode to Alternate function
			GPIOB->MODER &= ~((3UL << 12) |  (3UL << 14));
			GPIOB->MODER |=  ((2UL << 12) |  (2UL << 14));

			// Set the alternate function to AF04
			GPIOB->AFR[0] &= ~((15UL << 28)|(15UL << 24));
			GPIOB->AFR[0] |=  ((4UL << 28)|(4UL << 24));

			// Set open Drain
			GPIOB->OTYPER |= ((1UL << 7)| (1UL << 6));

			// Set internal pull up
			GPIOB->PUPDR &= ~((3UL << 12) |  (3UL << 14));
			GPIOB->PUPDR |=  ((1UL << 12) |  (1UL << 14));
		}
	}else if(pI2Cx->I2Cx == I2C2){
		// Pins are PB10-11
		SCL_PIN = 10;
		SDA_PIN = 11;
		// Set Pin mode to Alternate function
		GPIOB->MODER &= ~((3UL << 20) |  (3UL << 22));
		GPIOB->MODER |=  ((2UL << 20) |  (2UL << 22));

		// Set the alternate function to AF04
		GPIOB->AFR[1] &= ~((15UL << 8)|(15UL << 12));
		GPIOB->AFR[1] |=  ((4UL << 8)|(4UL << 12));

		// Set open Drain
		GPIOB->OTYPER |= ((1UL << 11)| (1UL << 10));

		// Set internal pull up
		GPIOB->PUPDR &= ~((3UL << 20) |  (3UL << 22));
		GPIOB->PUPDR |=  ((1UL << 20) |  (1UL << 22));
	}



}

void I2C_Init(I2C_Instance_T * pI2Cx){
	I2C_Gpio_Init(pI2Cx);

	uint32_t APB1_Clock_Speed = Get_APB1_ClockSpeed();

	if(APB1_Clock_Speed == 0){
		printf("Clock Speed must be > 2MHz");
		return;
	}
	// TODO Implement FM mode calculations
	float T_PCLK1 = (1 / APB1_Clock_Speed) * 1000000000; // period in NANO seconds
	uint32_t T_Low = 5000;
	uint32_t Max_Rise_T = 1000;

	if(pI2Cx->I2Cx == I2C1){

		// Enable the clock
		I2C1_PCLCK_EN();

		// Make sure I2C is disabled
		I2C1->CR1 &= ~(1UL << I2C_PE_BIT_POS);
		// Give the peripheral clock Frequency
		I2C1->CR2 |= ((APB1_Clock_Speed / 1000000) << I2C_FREQ_BIT_POS);
		// Calculate the Clock control register
		I2C1->CCR |= ( ((int)(T_Low / T_PCLK1)) << I2C_CCR_BIT_POS);
		// Calculate the rise time based on Max SCL Rise time in SM mode 1000 / T_PCLCK1
		I2C1->TRISE |= ((int)((Max_Rise_T / T_PCLK1) +1) << 0);

		// Enable the I2C Peripheral
		I2C1->CR1 |= (1UL << I2C_PE_BIT_POS);

	}else if(pI2Cx->I2Cx == I2C2){

		I2C2_PCLCK_EN();
		// Make sure I2C is disabled
		I2C2->CR1 &= ~(1UL << I2C_PE_BIT_POS);
		// Give the peripheral clock Frequency
		I2C2->CR2 |= ((APB1_Clock_Speed / 1000000) << I2C_FREQ_BIT_POS);
		// Calculate the Clock control register
		I2C2->CCR |= ( ((int)(T_Low / T_PCLK1)) << I2C_CCR_BIT_POS);
		// Calculate the rise time based on Max SCL Rise time in SM mode 1000 / T_PCLCK1
		I2C2->TRISE |= ((int)((Max_Rise_T / T_PCLK1) +1) << 0);

		// Enable the I2C Peripheral
		I2C2->CR1 |= (1UL << I2C_PE_BIT_POS);
	}




}

uint32_t Get_APB1_ClockSpeed(){

	uint8_t APB_Clck_divisions[5] = {1, 2, 4, 8, 16};
	uint16_t AHB_Clck_divisions[9] = {1, 2, 4, 8, 16, 64, 128, 256, 512};
	uint32_t APB1_Freq = 0;

	uint32_t SYS_clock_SRC  = RCC->CFGR & (3UL << 2);
	uint32_t APB1_Presacaler = RCC->CFGR & (7UL << 10);
	uint32_t AHB_Presacaler = RCC->CFGR & (15UL << 4);
	uint32_t HSI_Freq = 16000000;
	uint32_t HSE_Freq = 25000000;


	switch (SYS_clock_SRC){

	case 0:
		// HSI
		if(AHB_Presacaler < 4){
			if(APB1_Presacaler  == 0){
				APB1_Freq =  HSI_Freq / APB_Clck_divisions[APB1_Presacaler];
			}else{
				APB1_Freq =  HSI_Freq / APB_Clck_divisions[APB1_Presacaler -3];
			}

		}else{
			if(APB1_Presacaler  == 0){
				APB1_Freq =  (HSI_Freq / AHB_Clck_divisions[AHB_Presacaler - 7]) / APB_Clck_divisions[APB1_Presacaler];
			}else{
				APB1_Freq =  (HSI_Freq / AHB_Clck_divisions[AHB_Presacaler - 7]) / APB_Clck_divisions[APB1_Presacaler -3];
			}
		}

	case 1:
		// HSE
		if(AHB_Presacaler < 4){
			if(APB1_Presacaler  == 0){
				APB1_Freq =  HSE_Freq / APB_Clck_divisions[APB1_Presacaler];
			}else{
				APB1_Freq =  HSE_Freq / APB_Clck_divisions[APB1_Presacaler -3];
			}

		}else{
			if(APB1_Presacaler  == 0){
				APB1_Freq =  (HSE_Freq / AHB_Clck_divisions[AHB_Presacaler - 7]) / APB_Clck_divisions[APB1_Presacaler];
			}else{
				APB1_Freq =  (HSE_Freq / AHB_Clck_divisions[AHB_Presacaler - 7]) / APB_Clck_divisions[APB1_Presacaler -3];
			}
		}

	case 2:
		// TODO Calculations for PLL clock.
		// PLL


	};
	if(APB1_Freq < 2000000){
		return 0;
	}else{
		return APB1_Freq;
	}



}

void I2C_Burst_Write(I2C_TypeDef * tI2Cx, uint8_t slave_bse_add, uint8_t* msg, size_t size ){

	// Wait for bus to be free
	while(tI2Cx->SR1 & (1UL << I2C_BUSY_BIT_POS)){}

	// Generate Start condition
	Gen_Start_condition(tI2Cx);

	// Wait for start condition to finish
	while( !(tI2Cx->SR1 & (1UL << I2C_SB_BIT_POS)) ){}

	// Send slave address for write mode
	tI2Cx->DR = slave_bse_add << 1;

	// Wait for Data register to be empty
	while(tI2Cx->SR1 & (1UL <<I2C_TXE_BIT_POS )){}

	if(tI2Cx->SR1 & (1UL << I2C_ADDR_BIT_POS)){
		for(int i=0; i < size; i++){
			// Wait for Data register to be empty
			while(tI2Cx->SR1 & (1UL <<I2C_TXE_BIT_POS )){}
			// Send next byte
			tI2Cx->DR = *msg++;
		}
	}


}

void Gen_Start_condition(I2C_TypeDef * tI2Cx){
	tI2Cx->CR1 |= (1UL << I2C_START_BIT_POS);
}

void Gen_Stop_condition(I2C_TypeDef * tI2Cx){
	tI2Cx->CR1 |= (1UL << I2C_STOP_BIT_POS);
}










