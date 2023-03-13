#include <systick.h>


#define LOAD_VAL 	 16000    // num clock ticks per millisecond
#define ENABLE 		 (1U<<0)
#define SET_INTERUPT (1U<<1)
#define CLKSOURCE 	 (1U<<2)  // processor clock 1 : external clck 0
#define COUNTFLAG	 (1U<<16) // one if counter counted to zero since the last time it was read.
#define DISABLE		 (0)


void Delay(int ms){
	/* Configure systick*/
	SysTick->LOAD = LOAD_VAL;

	/* Clear systick current val register*/
	SysTick->VAL = 0;

	/* Enable and select clock*/
	SysTick->CTRL = (ENABLE | CLKSOURCE);

	/* Loop number of time equal to ms*/
	for(int i = 0; i < ms; i++ ){
		while(!(SysTick->CTRL & COUNTFLAG)){}
	}

	/* Disable the systick when finished*/
	SysTick->CTRL = DISABLE;
}


void systick_1hz_interupt(void)
{
	/* Set num ticks per second val*/
	SysTick->LOAD = 16000000 -1;

	/* Clear systick current val register*/
	SysTick->VAL = 0;

	/* Enable and select clock*/
	SysTick->CTRL = (ENABLE | CLKSOURCE | SET_INTERUPT);

}
