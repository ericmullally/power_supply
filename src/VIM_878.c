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

void load_integers(void){

}

