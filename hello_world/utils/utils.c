/* utils.c
 * Host several supporting functions like access to platform gpio resources, delay or prbs generation
 *
 *
 *  Created on: 07.02.2019
 *      Author: Thomas Erforth
 */

#include "utils.h"

#define NOISELEVEL 8000 // pseudo random generation



/* gpio_set: Sets the output level for the 3 color LED and the testpin
 * Eases the use of the PDL gpio macros
 */
void gpio_set(userGPIO gpio, uint8_t level)
{
	switch (gpio)
	{
		case TEST_PIN: 	Gpio1pin_Put(GPIO1PIN_P10, level); break;
		case LED_R: 	Gpio1pin_Put(GPIO1PIN_P1A, level); break;
		case LED_G: 	Gpio1pin_Put(GPIO1PIN_PB2, level); break;
		case LED_B:		Gpio1pin_Put(GPIO1PIN_P18, level); break;
		case USER_BUTTON: break;		// is input, no need to set
	}
}  // gpio_set


/* gpio_get: Return the input level
 * Eases the use of the PDL gpio macros
 */
uint8_t gpio_get(userGPIO gpio)
{
	switch (gpio)
	{
		case USER_BUTTON: 	return(Gpio1pin_Get(GPIO1PIN_P20));
		case LED_R:			return(Gpio1pin_Get(GPIO1PIN_P1A));
		case LED_G:			return(Gpio1pin_Get(GPIO1PIN_PB2));
		case LED_B:			return(Gpio1pin_Get(GPIO1PIN_P18));
		case TEST_PIN: 		return(Gpio1pin_Get(GPIO1PIN_P10));
	}
	return(0x0);

} // gpio_get


/* write_uart0: Simply transmits a string to UART0 transmit buffer
 *
 */
void writeUart0(uint8_t message[])
{
	uint8_t u8Cnt = 0;

	while(message[u8Cnt] != 0x0)
	    {
	        while (TRUE != Mfs_Uart_GetStatus(&UART0, UartTxEmpty)); //wait until TX buffer empty
	        Mfs_Uart_SendData(&UART0, message[u8Cnt]);

	        u8Cnt++;
	    }
} // write_uart0


/* prbs: gen pseudo-random sequence {-1,1}
 * copyright ARM University Program &copy; ARM Ltd 2015.
 */

short prbs(void) 			     			  		//gen pseudo-random sequence {-1,1}
{
	typedef struct BITVAL    // used in function prbs()
	{
	 unsigned short b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1;
	 unsigned short b7:1, b8:1, b9:1, b10:1, b11:1, b12:1,b13:1;
	 unsigned short dweebie:2; //Fills the 2 bit hole - bits 14-15
	} bitval;

	typedef union SHIFT_REG
	{
	 unsigned short regval;
	 bitval bt;
	} shift_reg;

	int fb = 1;                        		// feedback variable
	shift_reg sreg = {0xFFFF};         		// shift register

	int prnseq;

	if(sreg.bt.b0) prnseq = -NOISELEVEL;         //scaled negative noise level
	else prnseq = NOISELEVEL;		            //scaled positive noise level

	fb =(sreg.bt.b0)^(sreg.bt.b1);      //XOR bits 0,1
	fb^=(sreg.bt.b11)^(sreg.bt.b13);    //with bits 11,13 -> fb

	sreg.regval<<=1;
	sreg.bt.b0=fb;			    				    //close feedback path

	return prnseq;			   				      //return noise sequence value
}

/*
 * delay_ms, delay_us, delay_cycles
 * Generic delay functions.
 * copyright ARM University Program &copy; ARM Ltd 2014.
 */

void delay_ms(unsigned int ms) {
 unsigned int max_step = 1000 * (UINT32_MAX / CLK_FREQ);
 unsigned int max_sleep_cycles = max_step * (CLK_FREQ / 1000);
 while (ms > max_step) {
  ms -= max_step;
  delay_cycles(max_sleep_cycles);
 }
 delay_cycles(ms * (CLK_FREQ / 1000));
}

void delay_us(unsigned int us) {
 unsigned int max_step = 1000000 * (UINT32_MAX / CLK_FREQ);
 unsigned int max_sleep_cycles = max_step * (CLK_FREQ / 1000000);
 while (us > max_step) {
  us -= max_step;
  delay_cycles(max_sleep_cycles);
 }
 delay_cycles(us * (CLK_FREQ / 1000000));
}


#pragma GCC push_options
#pragma GCC optimize ("O0")
void delay_cycles(unsigned int cycles)
{
	asm("LSRS r0, #2");
	asm("BEQ done");
	asm("loop:");
	asm("SUBS r0, #1");
#if __CORTEX_M == 3 || __CORTEX_M == 4
	asm("NOP");
#endif
	asm("BNE loop");
	asm("done:");
 	//asm("BX lr");
}
#pragma GCC pop_options
