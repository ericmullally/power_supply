/*
 * lcd.c
 *
 *  Created on: Mar 8, 2023
 *      Author: eric
 */

#include <lcd.h>
/*
 * D7 PC7 		board D9
 * D6 PC8		board CN10 P2
 * D5 PC9		board CN10 P1
 * D4 PA8		board D7
 * D3 PA9		board D8
 * D2 PA10		board D2
 * D1 PA11		board CN10 P14
 * D0 PA12		board CN10 P12
 *
 * EN PB6		board D10
 * R/W PC15		board CN7 P27
 * RS PB14		board CN10 28
 * */

#define PORTA_CLCK_EN  (1U<<0)
#define PORTB_CLCK_EN  (1U<<1)
#define PORTC_CLCK_EN  (1U<<2)

#define D7_PORT 		GPIOC
#define D7_PIN_NUM		7
#define D6_PORT 		GPIOC
#define D6_PIN_NUM		8
#define D5_PORT 		GPIOC
#define D5_PIN_NUM		9
#define D4_PORT 		GPIOA
#define D4_PIN_NUM		8
#define D3_PORT 		GPIOA
#define D3_PIN_NUM		9
#define D2_PORT 		GPIOA
#define D2_PIN_NUM		10
#define D1_PORT 		GPIOA
#define D1_PIN_NUM		11
#define D0_PORT 		GPIOA
#define D0_PIN_NUM		12
#define LCD_EN_PORT		GPIOB
#define LCD_EN_PIN		6
#define LCD_RW_PORT		GPIOC
#define LCD_RW_PIN		15
#define LCD_RS_PORT		GPIOB
#define LCD_RS_PIN		14

#define CLEAR_DISPLAY 	0x01
#define RETURN_HOME 	0x02
#define DISPLAY_ON		0x0F  /// last 3 bits from msb  display on, cursor on, cursor position on

char cmnd  = 0x39;
char cmnd1 = 0x15;
char cmnd2 = 0x55;
char cmnd3 = 0x6D;
char cmnd4 = 0x78;
char cmnd5 = 0x30;
char cmnd6 = 0x0F;
char cmnd7 = 0x01;
char cmnd8 = 0x06;



void lcd_gpio_init(void){
	/* Enable clock access for ports A, B, and C*/
	RCC->AHB1ENR |= (PORTA_CLCK_EN | PORTB_CLCK_EN | PORTC_CLCK_EN);

	/* Set all pins to output mode*/
	GPIOA->MODER &= ~(3U<<16 | 3U<<18 | 3U<<20 | 3U<<22 | 3U<<24 );
	GPIOA->MODER |= (1U<<16 | 1U<<18 | 1U<<20 | 1U<<22 | 1U<<24 );

	GPIOB->MODER &= ~(3U<<28 | 3U<<12);
	GPIOB->MODER |= (1U<<12 | 1U<<28);

	GPIOC->MODER &= ~(3U<<14 | 3U<<16 | 3U<<18 | 3U<<30);
	GPIOC->MODER |= (1U<<14 | 1U<<16 | 1U<<18 | 1U<<30);

	/* Set PA12 output speed to low speed it matches the others */
	GPIOA->OSPEEDR &= ~(3U<<24);

}

void lcd_init(void){
	lcd_send_cmnd(cmnd);
	lcd_send_cmnd(cmnd1);
	lcd_send_cmnd(cmnd2);
	lcd_send_cmnd(cmnd3);
	lcd_send_cmnd(cmnd4);
	lcd_send_cmnd(cmnd5);
	lcd_send_cmnd(cmnd6);
	lcd_send_cmnd(cmnd7);
	lcd_send_cmnd(cmnd8);
}

void lcd_write_char(char character){
	// wait for lcd not busy
	lcd_read_bsy();

	//register select is set high
	LCD_RS_PORT->BSRR |= (1U<<LCD_RS_PIN);

	// rw is set low
	LCD_RW_PORT->BSRR |= (1U<<(LCD_RW_PIN + 16));


	// wait 1ms to be sure rw and rs are set
	Delay(1);


	// write data
	D0_PORT->BSRR |= (character & 0x01) ? (1U<< D0_PIN_NUM) : (1U<<(D0_PIN_NUM + 16));
	D1_PORT->BSRR |= (character & 0x02) ? (1U<< D1_PIN_NUM) : (1U<<(D1_PIN_NUM + 16));
	D2_PORT->BSRR |= (character & 0x04) ? (1U<< D2_PIN_NUM) : (1U<<(D2_PIN_NUM + 16));
	D3_PORT->BSRR |= (character & 0x08) ? (1U<< D3_PIN_NUM) : (1U<<(D3_PIN_NUM + 16));
	D4_PORT->BSRR |= (character & 0x10) ? (1U<< D4_PIN_NUM) : (1U<<(D4_PIN_NUM + 16));
	D5_PORT->BSRR |= (character & 0x20) ? (1U<< D5_PIN_NUM) : (1U<<(D5_PIN_NUM + 16));
	D6_PORT->BSRR |= (character & 0x40) ? (1U<< D6_PIN_NUM) : (1U<<(D6_PIN_NUM + 16));
	D7_PORT->BSRR |= (character & 0x80) ? (1U<< D7_PIN_NUM) : (1U<<(D7_PIN_NUM + 16));

	// set en high
	LCD_EN_PORT->BSRR |= (1U<<LCD_EN_PIN);

	// wait 1ms
	Delay(1);

	//set en low
	LCD_EN_PORT->BSRR |= (1U<<(LCD_EN_PIN +16));
	// wait 1ms
	Delay(500);
}

void lcd_send_cmnd(char cmnd){
	//register select is set low
	LCD_RS_PORT->BSRR |= (1U<<(LCD_RS_PIN +16));

	// rw is set low
	LCD_RW_PORT->BSRR |= (1U<<(LCD_RW_PIN + 16));

	// wait 1ms
	Delay(1);

	// write cmnd
	D0_PORT->BSRR |= (cmnd & 0x01) ? (1U<< D0_PIN_NUM) : (1U<<(D0_PIN_NUM + 16));
	D1_PORT->BSRR |= (cmnd & 0x02) ? (1U<< D1_PIN_NUM) : (1U<<(D1_PIN_NUM + 16));
	D2_PORT->BSRR |= (cmnd & 0x04) ? (1U<< D2_PIN_NUM) : (1U<<(D2_PIN_NUM + 16));
	D3_PORT->BSRR |= (cmnd & 0x08) ? (1U<< D3_PIN_NUM) : (1U<<(D3_PIN_NUM + 16));
	D4_PORT->BSRR |= (cmnd & 0x10) ? (1U<< D4_PIN_NUM) : (1U<<(D4_PIN_NUM + 16));
	D5_PORT->BSRR |= (cmnd & 0x20) ? (1U<< D5_PIN_NUM) : (1U<<(D5_PIN_NUM + 16));
	D6_PORT->BSRR |= (cmnd & 0x40) ? (1U<< D6_PIN_NUM) : (1U<<(D6_PIN_NUM + 16));
	D7_PORT->BSRR |= (cmnd & 0x80) ? (1U<< D7_PIN_NUM) : (1U<<(D7_PIN_NUM + 16));

	// set en high
	LCD_EN_PORT->BSRR |= (1U<<LCD_EN_PIN);

	// wait 1ms
	Delay(1);

	//set en low
	LCD_EN_PORT->BSRR |= (1U<<(LCD_EN_PIN +16));
	// wait 1ms
	Delay(1);
}

void lcd_read_bsy(void){
	//register select is set low
	LCD_RS_PORT->BSRR |= (1U<<(LCD_RS_PIN +16));

	// rw is set high
	LCD_RW_PORT->BSRR |= (LCD_RW_PIN);

	// read db7 while pulsing en
	while(D7_PORT->IDR & (1U<<D7_PIN_NUM)){

		LCD_EN_PORT->BSRR |= (1U<<LCD_EN_PIN);
		Delay(1);
		LCD_EN_PORT->BSRR |= (1U<<(LCD_EN_PIN +16));
		Delay(1);
	}
}










