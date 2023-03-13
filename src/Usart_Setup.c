#include <Usart_Setup.h>


#define PA2_SET_AFR_USART2_TX 		(0x7UL<<8) // set pa2 alternate function to USART TX
#define PA3_SET_AFR_USART2_RX 		(0x7UL<<12) // set pa3 alternate function to USART RX
#define SET_AFR_USART				(PA2_SET_AFR_USART2_TX + PA3_SET_AFR_USART2_RX)
#define MODER_TX					(0x1UL<<5)
#define MODER_RXTX					(0xA0UL)
#define USART_RX_IE					(1U<<5)

#define SYS_FREQ					16000000
#define BAUD_RATE					115200


static void set_usart_baud(USART_TypeDef *USARTx, uint32_t freq, uint32_t rate);
static uint16_t set_baud(uint32_t freq, uint32_t rate);
static void usart_write_char(uint8_t ch);


int __io_putchar(int ch){
	usart_write_char(ch);
	return ch;
}

void usart2_Tx_init(void){

	RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN_Msk;
	GPIOA->MODER  |= MODER_TX;
	GPIOA->AFR[0] |= PA2_SET_AFR_USART2_TX;
	RCC->APB1ENR  |= RCC_APB1ENR_USART2EN_Msk;

	set_usart_baud(USART2, SYS_FREQ, BAUD_RATE);
	USART2->CR1 = USART_CR1_TE;
	USART2->CR1 |= USART_CR1_UE;
}

void usart2_RxTx_init(void){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_Msk;
	GPIOA->MODER |= MODER_RXTX;
	GPIOA->AFR[0] |= SET_AFR_USART;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN_Msk;

	set_usart_baud(USART2, SYS_FREQ, BAUD_RATE);
	USART2->CR1 = (USART_CR1_TE | USART_CR1_RE);
	USART2->CR1 |= USART_CR1_UE;
}

char usart_read(void){
	// wait until data register is not empty
	while(!(USART2->SR & (1U<<5))){}
	return USART2->DR;
}

static void usart_write_char(uint8_t ch){
	while(!(USART2->SR & (1U<<7))){}
	USART2->DR = (ch & 0xff);
}

static void set_usart_baud(USART_TypeDef *USARTx, uint32_t freq, uint32_t rate){
	USARTx->BRR = set_baud(freq, rate);
}

static uint16_t set_baud(uint32_t freq, uint32_t rate){
	return ( (freq + (rate/2U))/ rate );
}

void usart2_Rx_interupt_init(void){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_Msk;
	GPIOA->MODER |= MODER_RXTX;
	GPIOA->AFR[0] |= SET_AFR_USART;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN_Msk;

	set_usart_baud(USART2, SYS_FREQ, BAUD_RATE);
	USART2->CR1 = (USART_CR1_TE | USART_CR1_RE);

	USART2->CR1 |= USART_RX_IE;

	NVIC_EnableIRQ(USART2_IRQn);

	USART2->CR1 |= USART_CR1_UE;
}
