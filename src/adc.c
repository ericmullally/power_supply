/*
 * adc.c
 *
 *  Created on: Mar 10, 2023
 *      Author: eric
 */
#include <adc.h>

/* ADC in1 APB2
 * PA1  A1 on board
 *
 *8bit conversion = 256 steps @ 3.3 v
 * 12.9mv per step or .0129v per step
 * */


#define GPIOA_CLCK_EN 				(1U<<0)
#define ADC_CLCK_EN					(1U<<8)
#define ADC_RES						(2U<<24)
#define ADC_LEFT_ALIGN				(1U<<11)
#define ADC_CONT_CONVERSION_MODE	(1U<<1)
#define ADC_NUM_CONVERSIONS			~(15U<<20) /// 0000 1 conversion  num conversions =  4 bit num + 1
#define ADC_PCLCK_DIV				(1U<<16)
#define ADC_EN						(1U<<0)
#define ADC_START_CONVERSION		(1U<<30)
#define ADC_EOC						(1U<<1)


const double volts_per_step = 0.0129;


void adc_gpio_init(void){
	/* Enable clock access to port A*/
	RCC->AHB1ENR |= GPIOA_CLCK_EN;

	/* Set PA1 to analog*/
	GPIOA->MODER |= (3U<<2);
}


void adc_init(void){
	adc_gpio_init();

	/* Enable clock access for adc*/
	RCC->APB2ENR |= ADC_CLCK_EN;

	/* Set resolution to 8 bit*/
	ADC1->CR1 &= ~(3U<<24);
	ADC1->CR1 |= ADC_RES;

	/* Set data alignment to right this moves zeros to the left*/
	ADC1->CR2 &= ~ADC_LEFT_ALIGN;

	/* Set single conversion mode*/
	ADC1->CR2 &= ~ADC_CONT_CONVERSION_MODE;

	/* Set number of converisons*/
	ADC1->SQR3 = 1;

	/* Enable the ADC periph*/
	ADC1->CR2 |= ADC_EN;
}


float convert_reading_to_float(int reading){
	return (reading * volts_per_step);
}


void start_single_conversion(void){
	ADC1->CR2 |= ADC_START_CONVERSION;
}


double read_conversion(void){
	// Wait for conversion to finish
	while(!(ADC1->SR & ADC_EOC)){}

	// convert the reading to a float and return it
	return convert_reading_to_float(ADC1->DR);
}










