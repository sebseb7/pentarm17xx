#include <stdlib.h>


#include "main.h"
#include "drivers/uart0.h"
#include "drivers/lcd.h"
#include "drivers/ssp.h"
#include "libs/text.h"

#include "drivers/lpc17xx_i2c.h"
#include "drivers/lpc17xx_pinsel.h"

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



	PINSEL_CFG_Type pincfg;               // for configuring pins
	pincfg.Portnum=PINSEL_PORT_0;              // I2C pins are all port 0
	pincfg.OpenDrain=PINSEL_PINMODE_OPENDRAIN; // as per $19.1
	pincfg.Pinmode=PINSEL_PINMODE_TRISTATE;    // (external pullup)
	pincfg.Funcnum=PINSEL_FUNC_3;     // function 3 = I2C1
	pincfg.Pinnum=19;               // SDA1
	PINSEL_ConfigPin(&pincfg);
	pincfg.Pinnum=20;               // SCL1
	PINSEL_ConfigPin(&pincfg);

	I2C_Init(LPC_I2C1, 100000);           // bus initialize, with speed
	I2C_Cmd(LPC_I2C1, ENABLE);        // bus activate


	uint8_t put = 0x32;
	uint8_t get[6] = {1,2,3,4,5,6};

	I2C_M_SETUP_Type blok;           // Master setup control block
	blok.sl_addr7bit=0xa6;
	blok.tx_data=&put;
	blok.tx_length=1;
	blok.rx_data=get;
	blok.rx_length=6;
	blok.retransmissions_max=5;
	Status mtd;                      // value from LPC functions



	while(1)
	{
		mtd=I2C_MasterTransferData(LPC_I2C1, &blok, I2C_TRANSFER_POLLING);
		lcd_cls();
		print_num_5x3_at(1,1,get[0],6,' ',1);
		print_num_5x3_at(1,10,get[1],6,' ',1);
		print_num_5x3_at(1,20,get[2],6,' ',1);
		print_num_5x3_at(1,30,get[3],6,' ',1);
		print_num_5x3_at(1,40,get[4],6,' ',1);
		print_num_5x3_at(1,50,get[5],6,' ',1);
		delay_ms(100);
	}
}


