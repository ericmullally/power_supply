/* I2C 1 is on apb1 bus
 * I2C 1 SCL is on PB8 the alternate function is AF04
 * I2C 1 SDA is on PB9 the alternate function is AF04
 * */
#include <I2C.h>


#define FREQUENCY 			(1U<<4)  // 16mhz
#define I2C_100KHZ_SCL  	80		// change to 80 khz
#define SM_MAX_RISE_T   	17


#define I2C1_CLCK_EN		(1U<<21)
#define GPIOB_CLCK_EN		(1U<<1)
#define I2C_RST				(1U<<15)

#define I2C_EN				(1U<<0)
#define LINE_BUSY			(1U<<1)
#define I2C_START			(1U<<8)
#define I2C_ACK_EN			(1U<<10)

#define I2C_START_FLAG  	(1U<<0)
#define I2C_ADDRESS_FLAG	(1U<<1)
#define I2C_TXE_FLAG		(1U<<7) // data register empty flag
#define I2C_RXNE_FLAG		(1U<<6) // data register not empty receive mode flag
#define I2C_STOP_FLAG 		(1U<<9)
#define I2C_BTF_FLAG		(1U<<2) // Byte transfer finished


void __init_write_request(char sadd);
void __init_read_request(char sadd);


void i2c1_init(void){

	/* Enable clock access to port b*/
	RCC->AHB1ENR |= GPIOB_CLCK_EN;

	/* Set  Alternate function mode on PB8-9*/
	GPIOB->MODER &= ~((3U<<16)| (3U<<18));
	GPIOB->MODER |= ((1U<<17) | (1U<<19));


	/* Set PB8-9 output to open drain*/
	GPIOB->OTYPER |= (3U<<8);

	/* Set alternate function to AF04*/
	GPIOB->AFR[1] &= ~((15U<<0) | (15U<<4));
	GPIOB->AFR[1] |= ((4U<<0) | (4U<<4));

//	/* Enable pull up resistors on PB8-9*/
//	GPIOB->PUPDR |= ((1U<<16)|(1U<<18));
//	GPIOB->PUPDR &= ~((1U<<17)|(1U<<19));

	/* Enable clock access to I2C 1*/
	RCC->APB1ENR |= I2C1_CLCK_EN;

	/* Set I2C1 to reset mode*/
	I2C1->CR1 |= I2C_RST;

	/* Exit Reset*/
	I2C1->CR1 &= ~I2C_RST;

	/* Set the frequency of I2C1*/
	I2C1->CR2 |= FREQUENCY;


	/* Set CCR to 100khz*/
	I2C1->CCR |= I2C_100KHZ_SCL;

	/* Set the max rise time for the clock*/
	I2C1->TRISE |= SM_MAX_RISE_T;

	/* Enable I2C 1*/
	I2C1->CR1 |= I2C_EN;

}

/*
 * @brief Reads one byte from a target.
 * All Transmissions are in 8 bits or 2 bytes
 * */
void i2c_read_byte(char sadd, char memadd, char* data ){

	/* Wait for I2C line open*/
	while(I2C1->SR2 & LINE_BUSY){}

	__init_write_request(sadd);

	/* Send the memory address we want to read*/
	I2C1->DR = memadd;

	/* Wait for Data register to be empty*/
	while(!(I2C1->SR1 & I2C_TXE_FLAG)){}

	__init_read_request(sadd);

	/* Disable ACK*/
	I2C1->CR1 &= ~I2C_ACK_EN;

	/* Generate stop condition*/
	I2C1->CR1 |= I2C_STOP_FLAG;

	/* Wait for the receive data flag*/
	while(!(I2C1->SR1 & I2C_RXNE_FLAG)){}

	/* Write the received data to the passed data pointer*/
	*data++ = I2C1->DR;
}

/*
 * @brief Reads multiple bytes from a target.
 * */
void i2c_burst_read(char sadd, char memadd, int n, char* data ){

	/* Wait for I2C line open*/
	while(I2C1->SR2 & LINE_BUSY){}

	__init_write_request(sadd);

	/* Wait for Data register to be empty*/
	while(!(I2C1->SR1 & I2C_TXE_FLAG)){}

	/* Send the memory address we want to read*/
	I2C1->DR = memadd;

	/* Wait for Data register to be empty*/
	while(!(I2C1->SR1 & I2C_TXE_FLAG)){}

	__init_read_request(sadd);

	/* Enable ACK*/
	I2C1->CR1 |= I2C_ACK_EN;

	while(n > 0U){
		/* Check if last operation */
		if(n == 1U){
			/* Disable ACK*/
			I2C1->CR1 &= ~I2C_ACK_EN;

			/* Generate stop condition*/
			I2C1->CR1 |= I2C_STOP_FLAG;
		}

		/* Wait for the receive data flag*/
		while(!(I2C1->SR1 & I2C_RXNE_FLAG)){}

		/* Write the received data to the passed data pointer*/
		*data++ = I2C1->DR;

		n--;
	}

}

/*
 * @brief Writes multiple bytes to a target
 * */
void i2c_burst_write(char sadd, char memadd, int n, char* data){

	/* Wait for I2C line open*/
	while(I2C1->SR2 & LINE_BUSY){}

	__init_write_request(sadd);

	/* Wait for Data register to be empty*/
	while(!(I2C1->SR1 & I2C_TXE_FLAG)){}

	/* Send the memory address we want to write to*/
	I2C1->DR = memadd;

	for(int i = 0; i < n; i++){
		/* Wait for Data register to be empty*/
		while(!(I2C1->SR1 & I2C_TXE_FLAG)){}

		/* Write the next bit then increment the pointer*/
		I2C1->DR = *data++;

		}
	/* Wait for byte transfer to be received*/
	while(!(I2C1->SR1 & I2C_BTF_FLAG)){}

	/* Generate stop condition*/
	I2C1->CR1 |= I2C_STOP_FLAG;

}


void __init_write_request(char sadd){

	volatile int tmp;

	/* Initiate communication start*/
	I2C1->CR1 |= I2C_START;

	/* Wait for start condition to finish generating*/
	while(!(I2C1->SR1 & I2C_START_FLAG)){}

	/* Transmit slave address with write request*/
	I2C1->DR = sadd << 1;

	/* Wait for Address flag to be set*/
	while(!(I2C1->SR1 & I2C_ADDRESS_FLAG)){}

	/* Read SR2 register after reading sr1 to clear the address flag in SR1*/
	tmp = I2C1->SR2;
}

void __init_read_request(char sadd){

	volatile int tmp;

	/* Send another start condition*/
	I2C1->CR1 |= I2C_START;

	/* Wait for start condition to finish generating*/
	while(!(I2C1->SR1 & I2C_START_FLAG)){}

	/* Transmit read request*/
	I2C1->DR = sadd << 1 | 1;

	/* Wait for Address flag to be set*/
	while(!(I2C1->SR1 & I2C_ADDRESS_FLAG)){}

	/* Read SR2 register after reading sr1 to clear the address flag in SR1*/
	tmp = I2C1->SR2;

}


























