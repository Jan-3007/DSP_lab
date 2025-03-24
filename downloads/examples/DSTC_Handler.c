/* DSTC_Handler.c
 * Interrupt handler for DSTC channels 0 and 1
 * CH0 is used for I2S transmit, CH1 for I2S receive
 * Buffer size is defined in 'DMA_BUFFER_SIZE' in file 'platform.h'
 *
 *
 */

#include <platform.h>
#include <utils.h>

// user global variables
volatile uint8_t rx_proc_buffer = PING;
volatile uint8_t tx_proc_buffer = PING;
volatile boolean_t tx_buffer_empty = FALSE;
volatile boolean_t rx_buffer_full = FALSE;


/* DSTC_HANDLER_CH0: Interrupt handler for DSTC channel 0
 * Channel 0 is used for I2S transmit, toggles the buffer address
 * in channel 0 DES2 between tx ping and pong buffer and restarts
 * the transmission by clearing the DQmask bit for DSTC channel 219
 */
void DSTC_I2S_HANDLER_CH0 (void)
{
	// process tx buffer addresses
	if(tx_proc_buffer == (PONG))
	{
		stcDES[0].DES2 = (uint32_t)&(dma_tx_buffer_pong);	// source address: tx pong buffer
		tx_proc_buffer = PING;								// next buffer is ping
	}
	else
	{
		stcDES[0].DES2 = (uint32_t)&(dma_tx_buffer_ping);	// source address: tx ping buffer
		tx_proc_buffer = PONG;								// next buffer is pong
	}
	tx_buffer_empty = TRUE;                                 // signal to main() that tx buffer empty

	Dstc_SetDqmskclrBit(DSTC_IRQ_NUMBER_I2S0_TX);			// clear mask bit for channel 219 (I2S transmission)

	gpio_set(LED_B, LOW);			// LED_B on
}


/* DSTC_HANDLER_CH1: Interrupt handler for DSTC channel 1
 * Channel 1 is used for I2S receive, toggles the buffer address
 * in channel 1 DES2 between rx ping and pong buffer and restarts
 * the transmission by clearing the DQmask bit for DSTC channel 218
 */
void DSTC_I2S_HANDLER_CH1 (void)
{
	// process rx buffer addresses
   	if(rx_proc_buffer == PONG)
   	{
   		stcDES[1].DES3 = (uint32_t)&(dma_rx_buffer_pong);	// receive address: rx pong buffer
		rx_proc_buffer = PING;								// next buffer is ping
	}
	else
	{
		stcDES[1].DES3 = (uint32_t)&(dma_rx_buffer_ping);	// receive address: rx ping buffer
		rx_proc_buffer = PONG;								// next buffer is ping
	}
	rx_buffer_full = TRUE;									// signal to main() that rx buffer is full

	Dstc_SetDqmskclrBit(DSTC_IRQ_NUMBER_I2S0_RX);			// clear mask bit for channel 218 (I2S reception)

	gpio_set(LED_B, LOW);			// LED_B on
}
