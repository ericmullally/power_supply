/*
 * lcd.h
 *
 *  Created on: Mar 8, 2023
 *      Author: eric
 */

#ifndef LCD_H_
#define LCD_H_

#include <stm32f4xx.h>
#include <systick.h>



void lcd_gpio_init(void);
void lcd_init(void);
void lcd_write_char(char character);
void lcd_send_cmnd(char cmnd);
void lcd_read_bsy(void);

#endif /* LCD_H_ */
