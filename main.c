#include <stdlib.h>


#include "main.h"
#include "drivers/uart0.h"
#include "drivers/lcd.h"
#include "drivers/ssp.h"
#include "libs/text.h"

volatile uint32_t timeout_ms = 0;
volatile uint32_t timeout_delay_ms = 0;



void SysTick_Handler(void) 
{
	if(timeout_ms > 0) timeout_ms--;
	if(timeout_delay_ms > 0) timeout_delay_ms--;
}



void delay_ms(uint32_t delay_period_ms)
{
	timeout_delay_ms = delay_period_ms;
	while(timeout_delay_ms > 0);
}


int main(void) 
{
	// don't know why this is incorrect
	SystemCoreClock = 100000000;
	SysTick_Config(SystemCoreClock / 1000);

	timeout_ms=0;
	
	UART0_Init(115200);


	sspInit();
	lcdInit();


	// sets SystemCoreClock to 44583722, but why ? 
	SystemCoreClockUpdate();


	lcd_plot (1, 1, 1);
	print_5x3_at(3,3,"Hello",1);

	while(1)
	{
	}
}


