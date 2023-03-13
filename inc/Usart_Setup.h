/*
 * Usart_printf.h
 *
 *  Created on: Jan 31, 2023
 *      Author: Eric
 */

#ifndef USART_SETUP_H_
#define USART_SETUP_H_

#include <stm32f4xx.h>
#include <stdint.h>

void usart2_Tx_init(void);
void usart2_RxTx_init(void);
char usart_read(void);
void usart2_Rx_interupt_init(void);

#endif /* USART_SETUP_H_ */

