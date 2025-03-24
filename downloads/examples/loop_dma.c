/* loop_dma.c
 * Demonstrates the use of descriptor base DMA on the FM4. Simply receives and transmits audio samples block wise.
 * Compile together with DSTC_Handler.c
 *
 *  * Recommended slider app parameter settings
 * Index	Name		Minimum		Initial		Maximum		Step Size		Digits
 * 0
 * 1
 * 2
 * 3
 * 4		Line in		0			23			31			1				1.0
 * 5		HP Out		48			48			127			1				1.0
 *
 *
 *
 *
 */

#include <pdl_header.h>
#include <platform.h>
#include <utils.h>

// user definitions
#define SAMPLERATE 48000	// CODEC sampling frequency in Hz, 8000, 32000, 48000 or 96000


// user global variables


// prototypes

void process_buffer(void);


int main (void)
{
	// Initialize platform resources: I2S, I2C and UART interfaces, CODEC, GPIO for LED, user button and testpin
	// Baudrate is defined in 'platform.h'
	platform_init(BAUDRATE, SAMPLERATE, line_in, dma, DSTC_I2S_HANDLER_CH0, DSTC_I2S_HANDLER_CH1);

	while(1)
	{
		while (!(rx_buffer_full && tx_buffer_empty))
		{
			// optional: Update slider parameters, needed in case the slider GUI is used

			update_slider_parameters(&FM4_GUI);

			// optional: update line in and head phone level through slider app
			// uses slider 4 and 5
			setLIandHPlevels(&FM4_GUI);

			// toggle activity indicator
			gpio_set(LED_B, HIGH);		// LED_B off
		}

		process_buffer();	// Algorithm
	}
}

/* process_buffer
 * Process the data received from DSTC interrupt handler (DSTC_Handler.c)
 * Caveat: Data processing must be complete before the next DAM buffer is available
 */
void process_buffer(void)
{
	int i;
	uint32_t *txbuf, *rxbuf;
	int16_t leftChannel, rightChannel;
	union audio audioIO;

//	gpio_set(TEST_PIN, HIGH);		// toggle TestPin P10 for computing time measurement

	// set txbuf and rxbuf pointer to the active DMA buffer
	// do not modify
	if(tx_proc_buffer == PING) txbuf = dma_tx_buffer_ping;
	else txbuf = dma_tx_buffer_pong;

	if(rx_proc_buffer == PING) rxbuf = dma_rx_buffer_ping;
	else rxbuf = dma_rx_buffer_pong;

	// enter your data processing algorithms here
	// in this example we just copy the content of the receive buffer into transmit buffer

	for(i=0; i < DMA_BUFFER_SIZE ; i++)
	{
		*txbuf++ = *rxbuf++;

		// example of extracting audio channel data from receive buffer,
		// each sample comprises 16 bit left and right channel data
		// audioIO.audioSample = *rxbuf++;			// fetch audio sample from receive buffer
		// leftChannel = audioIO.audio_ch[LEFT]; 	// extract left channel data
		// rightChannel = audioIO.audio_ch[RIGHT];	// extract right channel data

		// process data

		// audioIO.audio_ch[LEFT] =  leftChannel;			// write processed data
		// audioIO.audio_ch[RIGHT] = rightChannel;			// write processed data

		// *txbuf++ = audioIO.audioSample;
	}


	// Flags for DMA handling, do not modify
	tx_buffer_empty = 0;
	rx_buffer_full = 0;

//	gpio_set(TEST_PIN, LOW);		// toggle TestPin P10 for computing time measurement
}
