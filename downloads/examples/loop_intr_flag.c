/* loop_intr_flag.c
 * Simply receives and transmits audio samples. Can be used to check if the I2S interface is working as intended.
 * Audio data are processed in ISR
 *
 * Recommended slider app parameter settings
 * Index	Name		Minimum		Initial		Maximum		Step Size		Digits
 * 0
 * 1
 * 2
 * 3
 * 4		Line in		0			23			31			1				1.0
 * 5		HP Out		48			48			127			1				1.0
 *
 *
 * Change History:
 * ter; Jan 3, 2019:	Volume control for CODEC head phone out added, volume control in IS removed
 * ter; Jan 5, 2019:	Volume control for CODEC line in added, support of LRxxBOTH bit added.
 * 						Audio samples are now stored and accessed by means of a union (audio) (520 ns vs 870 ns processing time)
 * ter; Jan 31, 2019:	'init_LED' replaced by 'init_gpio',
 * 						gpio settings replaced by PDL 2.1.0 macros
 * 						Sampling frequency can now be set by means of variable 'SFREQUENCY'
 * 						GPIO and CODEC commands modified to support latest PDL drivers
 * ter; Feb 08, 2019: 	init_slider_interface() moved from main() to platform_init()
 * 						CODEC Line in and HP gain update moved to function setLIandHPlevels()
 *
 */

#include <pdl_header.h>
#include <platform.h>
#include <utils.h>

// user definitions
#define SAMPLERATE 48000	// CODEC sampling frequency in Hz, 8000, 32000, 48000 or 96000

// user global variables
int8_t flag = 0;
union audio audioIO;

// prototypes
void I2S_HANDLER(void);

void I2S_HANDLER(void) {   /****** I2S Interruption Handler *****/

	flag = 1;
	audioIO.audioSample = I2s_ReadRxFifo(&I2S0); // get one sample (left and right channel, 16 bit each), 175 ns

	//	I2s_WriteTxFifo(&I2S0, audioIO.audioSample); // send one sample, 175 ns
}

int main(void) {
//	union audio audioIO;

	// Initialize platform resources: I2S, I2C and UART interfaces, CODEC, GPIO for LED, user button and test pin
	platform_init(BAUDRATE, SAMPLERATE, line_in, intr, I2S_HANDLER, NULL); // second pointer to interrupt handler only for DSTC needed

	// send a string to the terminal
	// writeUart0("Hello FM4 World!\r\n");
	
	while (1) {
		if(1 == flag) {
			flag = 0; // first transaction: reset the ISR flag to zero

			//gpio_toggle(TEST_PIN);
			gpio_set(TEST_PIN, HIGH);		// TestPin P10, 110 ns

//			audioIO.audioSample = I2s_ReadRxFifo(&I2S0); // get one sample (left and right channel, 16 bit each), 175 ns

			//	Process (nothing to do, just a loop)

			//	audioIO.audio_ch[LEFT] =  ;
			//	audioIO.audio_ch[RIGHT] = ;

			I2s_WriteTxFifo(&I2S0, audioIO.audioSample); // send one sample, 175 ns

			gpio_set(TEST_PIN, LOW);		// TestPin P10, 110 ns
			gpio_set(LED_B, LOW);			// LED_B on

		}
	}
}
