/* platform.c
 * Platform initialization, enables access to platform resources like I2S0, audio CODEC, UART0 and gpio
 *
 *  Copyright (C) 2019 Thomas Erforth
 *
 * Change history:
 * ter, Jan 30, 2019: 	Initial version
 * ter, Apr 8, 2019: 	In function platform_init / I2S interface configuration intMode 'poll' added
 * ter, July 14, 2019: 	Function DstcInit, initialization changed to support outer loops i.e.
 * 						buffer sizes 1 - 256 plus multiples of 256
 * ter, July 21, 2019: 	Declarations for dma buffers, DSTC and slider app added (moved from platform.h)
 *
 */

#include "global.h"

/*  platform_init: Configures I2S, I2C and UART interfaces, CODEC, GPIOs for LED, user button and testpin.
 *  Calls various PDL drivers located in the 'PDL' folder
 *  INPUT:
 *      baud_rate: Choose appropriate value, typical values for a pc system are 921600, 460800, 230400, 115200 etc.
 * 	    sample_rate: hz8000, hz32000, hz48000 or hz96000, used for I2S and CODEC
 * 	    audio_in: Selects line_in or mic_in of the CODEC
 */
void init_platform(
    uint32_t baud_rate, 
    sampling_rate sample_rate, 
    audio_input audio_in
    )
{
// GPIO port configuration for 3 color LED, user button and test pin
	init_gpio();

//Activate UART0 which is used for slider app interface
	init_uart0(baud_rate);

// Activate I2C interface to CODEC and configure CODEC registers
	init_codec(sample_rate, audio_in);

// I2S interface configuration
	init_I2S0(sample_rate);

	//DSTC module initialization functions;
	//Configures DSTC channel 0 to transfer data from memory to I2S TX,
	// and channel 1 from I2S RX to memory.
	init_dstc();
}

void platform_start()
{
	I2s_StartClk(&I2S0);						// I2SEN = 1, clock to I2S macro disabled

	I2s_EnableTx(&I2S0);

	I2s_EnableRx(&I2S0);

	I2s_Start(&I2S0);
}


/* 
 * Activate I2C interface to CODEC and configure CODEC registers
 * CODEC works as I2S clock master
 * Input: 
 *      codec_sample_rate: 8 , 32, 44,1 or 48 KHz 
 *      audioIn: line in or mic in
 */
void init_codec(
    sampling_rate codec_sample_rate, 
    audio_input audio_in
    )
{
	// I2C interface pin function, I2C2
	SetPinFunc_SOT2_1();
    SetPinFunc_SCK2_1();

	stc_wm8731_reg_t wm8731_config; 	// instantiate the WM8731 register config structure
	PDL_ZERO_STRUCT(wm8731_config);		// clear

	// populate WM8731 register structure
	wm8731_config.RESET	= 0X0;
	wm8731_config.LLIN 	= 0x17;				//0x17, Left Line In. Select gain 0dB.
	wm8731_config.RLIN 	= 0x17;				//0x17, Right Line In. Select gain 0dB.
	wm8731_config.LHOUT = 0x78;				//Left Headphone Out. Select volume; 78=-1db, 7F=+6dB, 30=-73dB; 79 steps, 1dB each.
	wm8731_config.RHOUT = 0x78;				//Right Headphone Out (2F = mute)
	wm8731_config.AAPC 	= audio_in;			//Select input; LINE in or microphone in.
	wm8731_config.DAPC 	= 0x00 ; 			//Audio Path Control, high pass filter, no soft mute.
	wm8731_config.DAIF 	= 0x53; 			//Format; 16 bits; DSP data format. I2S format works for the s6e2CC with 48 KHz sampling rate only
	wm8731_config.SC 	= codec_sample_rate; 	//Sample Control; Mclk, 256fs, normal mode, and sampling frequency.
	wm8731_config.AC 	= 0x01; 			//Activate interface
	wm8731_config.PDC	= 0x02; 			//Power control ->Disable power down; all on except mic

	// Initialize I2C2 interface and CODEC
	Wm8731_Init(&I2C2, &wm8731_config);
}




// required for DSTC
stc_dstc_des0123456_t stcDES[2];     // Instance for DSTC Descriptors 0 - 4

/* 
 * Configures DSTC channel 0 to transfer data from memory to I2S TX, and channel 1 from I2S RX to memory.
 */
void init_dstc()
{
	stc_dstc_config_t stcDstcConfig;	// DSTC config structure

	// clear config and data structures
	PDL_ZERO_STRUCT(stcDstcConfig);
	PDL_ZERO_STRUCT(stcDES[0]);
	PDL_ZERO_STRUCT(stcDES[1]);

	while ( Ok != Dstc_ReleaseStandBy()){};	// wait till DSTC goes to normal state

	// populate descriptor structures

	// channel 0 = Reads from memory (dma_tx_buffer_ping and dma_tx_buffer_pong) and transfer to the I2S peripheral
	// CH0, DES0
	stcDES[0].DES0.DV    	= 0x03;		// Don't execute the DES close process after transfer ends
	stcDES[0].DES0.ST    	= 0u;		// Just a default, DSTC updates this on completion of transfer with status information
	stcDES[0].DES0.MODE  	= 1u;      	// Mode 1 -> single transfer for 1 trigger
	stcDES[0].DES0.ORL   	= 3u;      	// Outer reload for DES1, 2
	stcDES[0].DES0.TW    	= 2u;      	// 32-bit transfer width
	stcDES[0].DES0.SAC   	= 0u;      	// source address is increased by TW * 1 at every transfer without InnerReload
	stcDES[0].DES0.DAC   	= 5u;       // Destination address remains unchanged during the transfer
	stcDES[0].DES0.CHRS  	= 0x10u;    // Interrupt flag is set when IRM = 1 and ORM = 1. No Chain start
	stcDES[0].DES0.DMSET 	= 1u;       // Set DQMSK = 1 when DES close process is executed
	stcDES[0].DES0.CHLK  	= 0u;       // No Chain start transfer
	stcDES[0].DES0.ACK   	= 1u;       // Output DMA transfer acknowledge to peripheral
	stcDES[0].DES0.RESERVED = 0u;   	// Required
	stcDES[0].DES0.PCHK  	= DSTC_PCHK_CALC(stcDES[0].u32DES0);	// parity

	// CH0, DES1, counters can be set to 1 - 256, and multiples of 256      //TODO ??? can it be done differently
	stcDES[0].DES1_mode1.ORM = ((BLOCK_SIZE -1) >> 8) + 1;			// outer loop count
	stcDES[0].DES1_mode1.IIN = (stcDES[0].DES1_mode1.ORM > 1) ? 0 : BLOCK_SIZE & 0XFF;  // Inner loop, max 256; 256 = 0
	stcDES[0].DES1_mode1.IRM = stcDES[0].DES1_mode1.IIN;			// Same as IIN

	// CH0, DES2
	stcDES[0].DES2 = (uint32_t)get_new_tx_buffer_ptr() ;   	// Source address (incremented by TW * 1 for every transfer. Configured in DES0.SAC)

	// CH0, DES3
	stcDES[0].DES3 = (uint32_t)&FM4_I2S0->TXFDAT;      	// Destination address - I2S Transmission data register (Same for every transfer,
														// configured in DES0.DAC)
	// CH0, DES4
	stcDES[0].DES4_mode1 = stcDES[0u].DES1_mode1;		// Used to reload DES1

	// Ch0, DES5
	stcDES[0].DES5 = stcDES[0].DES2;					// Used to reload DES 2 in outer reload

	Dstc_SetHwdesp(DSTC_IRQ_NUMBER_I2S0_TX, 0);			// descriptor pointer start address at DESTP + offset 0 for HW channel 219



	// channel 1 = Reads from the I2S peripheral and transfer to (dma_rx_buffer_ping and dma_rx_buffer_pong)
	// CH1, DES0
	stcDES[1].DES0.DV    = 0x03;           	// Don't Execute the DES close process after transfer ends
	stcDES[1].DES0.ST    = 0u;             	// Just a default, DSTC updates this on completion of transfer with status information
	stcDES[1].DES0.MODE  = 1u;             	// Mode 1 -> single transfer for 1 trigger
	stcDES[1].DES0.ORL   = 5u;             	// Outer reload for DES1, 3
	stcDES[1].DES0.TW    = 0x2;            	// 32-bit transfer width
	stcDES[1].DES0.SAC   = 5u;             	// Source address remains unchanged during the transfer
	stcDES[1].DES0.DAC   = 0u;             	// Destination address is incremented by TW * 1 at every transfer without reload
	stcDES[1].DES0.CHRS  = 0x10u;          	// Interrupt flag is set when IRM = 1 and ORM = 1. No Chain start
	stcDES[1].DES0.DMSET = 1u;             	// Set DQMSK = 1 when DES close process is executed
	stcDES[1].DES0.CHLK  = 0u;             	// No Chain start transfer
	stcDES[1].DES0.ACK   = 1u;             	// Output DMA transfer acknowledge to peripheral
	stcDES[1].DES0.RESERVED = 0u;			// Required
	stcDES[1].DES0.PCHK  = DSTC_PCHK_CALC(stcDES[1u].u32DES0);

	// CH1, DES1, counters can be set to 1 - 256, and multiples of 256
	stcDES[1].DES1_mode1.ORM = ((BLOCK_SIZE -1) >> 8) + 1;		// outer loop count
	stcDES[1].DES1_mode1.IIN = (stcDES[1].DES1_mode1.ORM > 1) ? 0 : BLOCK_SIZE & 0XFF;  // Inner loop, max 256; 256 = 0
	stcDES[1].DES1_mode1.IRM = stcDES[1].DES1_mode1.IIN;   		// Same as IIN

	// CH1, DES2
	stcDES[1].DES2 = (uint32_t)&FM4_I2S0->RXFDAT ;     	// Source address

	// CH1, DES3
	stcDES[1].DES3 = (uint32_t)get_new_rx_buffer_ptr();   	// Destination address - I2S Transmission data register (Same for every transfer. Configured in DES0.DAC)

	// CH1, DES4
	stcDES[1].DES4_mode1 = stcDES[1].DES1_mode1;      	// used to reload DES1

	// CH1, DES6
	stcDES[1].DES6 = stcDES[1].DES3;					// Used to reload DES 3 in outer reload

	Dstc_SetHwdesp(DSTC_IRQ_NUMBER_I2S0_RX, 0x1C);		// descriptor pointer start address DESTP + offset 0x1C for HW channel 218
														// (7 DES0 x 4 Bytes each = 0x1C)



    // populate dstc config structure

	// DES Top, start Address of DES Area (must be aligned to 32 Bit!)
	stcDstcConfig.u32Destp = (uint32_t) &stcDES[0];
	// TRUE: Software Interrupt enabled
	stcDstcConfig.bSwInterruptEnable = FALSE;
	// TRUE: Error Interrupt enabled
	stcDstcConfig.bErInterruptEnable = FALSE;
	// TRUE: Read Skip Buffer disabled
	stcDstcConfig.bReadSkipBufferDisable = FALSE;		// no further changes made in DES0 after init
	// TRUE: Enables Error Stop
	stcDstcConfig.bErrorStopEnable = FALSE;
	// SW transfer priority
	stcDstcConfig.enSwTransferPriority = PriorityLowest;
	// TRUE: enable NVIC
	stcDstcConfig.bTouchNvic = TRUE;
	// interrupt handler
 	stcDstcConfig.pfnDstcI2s0TxCallback = isr_tx;		// pointer to interrupt service routine
	stcDstcConfig.pfnDstcI2s0RxCallback = isr_rx;


 	Dstc_Init(&stcDstcConfig);		// write config structure to CFG register and setup interrupt callback mechanism

 	Dstc_SetCommand(CmdSwclr);		// Command to clear the SWINT interrupt
	Dstc_SetCommand(CmdErclr);		// Command to clear ERINT interrupt. MONERS.EST = 0, MONERS.DER = 0, MONERS.ESTOP = 0
	Dstc_SetCommand(CmdMkclr);		// Command to clear all DQMSK[n] registers

	Dstc_SetDreqenbBit(DSTC_IRQ_NUMBER_I2S0_TX);		// enable HW channel 219
	Dstc_SetHwintclrBit(DSTC_IRQ_NUMBER_I2S0_TX);		// Clear HWINT6 register bit corresponding to HW channel 219(I2S tx)
	Dstc_SetDreqenbBit(DSTC_IRQ_NUMBER_I2S0_RX);		// enable HW channel 218
	Dstc_SetHwintclrBit(DSTC_IRQ_NUMBER_I2S0_RX);		// Clear HWINT6 register bit corresponding to HW channel 218 (I2S rx)
}

// DSTC ISR for TX 
void isr_tx()
{
    // transfer is done
    // get new buffer address to read from
    uint32_t* ptr = get_new_tx_buffer_ptr();

    if(ptr == nullptr)
    {
        // error: nothing to read
        fatal_error();
    }

    // new DSTC address
    stcDES[0].DES2 = reinterpret_cast<uint32_t>(ptr);

    // clear mask bit for channel 219 (I2S transmission), this reenables the DSTC
  	Dstc_SetDqmskclrBit(DSTC_IRQ_NUMBER_I2S0_TX);
}

// DSTC ISR for RX
void isr_rx()
{
    // transfer is done
    // get new buffer address to write to
    uint32_t* ptr = get_new_rx_buffer_ptr();

    if(ptr == nullptr)
    {
        // error: cannot write
        fatal_error();
    }

    // new DSTC address
    stcDES[1].DES3 = reinterpret_cast<uint32_t>(ptr);

    // clear mask bit for channel 218 (I2S reception), this reenables the DSTC
	Dstc_SetDqmskclrBit(DSTC_IRQ_NUMBER_I2S0_RX);			  
}




/* 
 * Populates the I2S0 configuration, interrupt enable and interrupt call back structures provided by PDL library,
 * sets pin function for I2s and configures I2S clock. Calls PDL drivers to initialize the interface.
 * Input: 
 *      sample_rate: hz8000, hz32000, hz48000 or hz96000
 */
void init_I2S0(sampling_rate sample_rate)
{
	stc_i2s_irq_en_t stci2s0irqen; 		// i2s irq enable structure
	stc_i2s_config_t stci2s0Cfg;		// i2s config structure
	stc_i2s_intern_data_t stci2s0cb;	// i2s interrupt call back structure

	// clear structure
	PDL_ZERO_STRUCT(stci2s0irqen);
	PDL_ZERO_STRUCT(stci2s0Cfg);
	PDL_ZERO_STRUCT(stci2s0cb);

	// enable bus clock for I2S0
	Clk_PeripheralClockEnable(ClkGateI2s0);

	// set pin function for I2S0
	SetPinFunc_I2SMCLK0_0_IN();	// I2S0-MCLK pin to input
	SetPinFunc_I2SDO0_0();		// I2S0-DO pin to output
	SetPinFunc_I2SDI0_0();		// I2S0-DI pin to input
	SetPinFunc_I2SCK0_0();		// I2S0-CK pin to input/output (input used)
	SetPinFunc_I2SWS0_0();		// I2S0-WS pin to input/output (input used)

	// configure I2S clock
	I2s_StopClk(&I2S0);				// I2SEN = 0, clock to I2S macro disabled
	I2s_DisablePll();				// IPLLEN = 0, PLL not used, clock comes from CODEC
	FM_I2SPRE->ICCR_f.ICSEL = 1u;	// ICSEL = 1, excerpt from function I2s_InitClk: MCLKI is input for clock
									// Function I2s_InitClk configures all the PLL registers before which is not necessary

	// populate i2s call back function structure

/*	// TX-FIFO underflow 0 interrupt
	stci2s0cb.pfnTxFifoUnderflow0IrqCb = NULL;
	// TX-FIFO underflow 1 interrupt
	stci2s0cb.pfnTxFifoUnderflow1IrqCb = NULL;
	// TX block size error interrupt
	stci2s0cb.pfnTxBlockSizeErrIrqCb = NULL;
	// frame error interrupt
	stci2s0cb.pfnFrameErrIrqCb = NULL;
	// TX-FIFO overflow interrupt
	stci2s0cb.pfnTxFifoOverflowIrqCb = NULL;
	// Enable TX FIFO interrupt
	stci2s0cb.pfnTxFifoIrqCb = NULL;
 	// RX block size error interrupt
	stci2s0cb.pfnRxBlockSizeErrIrqCb = NULL;
 	//RX-FIFO underflow interrupt
	stci2s0cb.pfnRxFifoUnderflowIrqCb = NULL;
 	//RX-FIFO overflow interrupt
	stci2s0cb.pfnRxFifoOverflowIrqCb = NULL;
	// RX FIFO idle detection interrupt
	stci2s0cb.pfnRxFifoIdleDetectIrqCb = NULL;*/
 	// Enable RX FIFO interrupt
	stci2s0cb.pfnRxFifoIrqCb = nullptr;


	// populate the i2s interrupt enable structure

	// TX-FIFO underflow 0 / 1 interrupt
	stci2s0irqen.bTxFifoUnderflow0Irq = FALSE;		//FALSE: disabled, caveat: PDL driver inverts this bit
	stci2s0irqen.bTxFifoUnderflow1Irq = FALSE;		// before writing to register
	// TX block size error interrupt
	stci2s0irqen.bTxBlockSizeErrIrq = FALSE;
	// frame error interrupt
	stci2s0irqen.bFrameErrIrq = FALSE;
	// TX-FIFO overflow interrupt
	stci2s0irqen.bTxFifoOverflowIrq = FALSE;
	//TX FIFO interrupt
	stci2s0irqen.bTxFifoIrq = FALSE;				// use receive FIFO interrupt only
	//RX block size error interrupt
	stci2s0irqen.bRxBlockSizeErrIrq = FALSE;
	// RX-FIFO underflow interrupt
	stci2s0irqen.bRxFifoUnderflowIrq = FALSE;
	// RX-FIFO overflow interrupt
	stci2s0irqen.bRxFifoOverflowIrq = FALSE;
	// RX FIFO idle detection interrupt
	stci2s0irqen.bRxFifoIdleDetectIrq = FALSE;
	// RX FIFO interrupt
	stci2s0irqen.bRxFifoIrq = FALSE;


	// populate the i2s init structure

	 stci2s0Cfg.u8ClockDiv = 0u;	// Bypass, use external clock as is

	 // overhead bits
	 if (sample_rate == hz8000)  stci2s0Cfg.u16OverheadBits = 352;
	 if (sample_rate == hz32000) stci2s0Cfg.u16OverheadBits = 64;
	 if (sample_rate == hz48000) stci2s0Cfg.u16OverheadBits = 32;
	 if (sample_rate == hz96000) stci2s0Cfg.u16OverheadBits = 96;

	 // mask bit: If the transmit FIFO is empty when the frame sync signal is received,
	 // MSKB is output in all valid channels of the transmit frame.
	 stci2s0Cfg.bMaskBit = FALSE;			// send '0'
	 // master / slave configuration
	 stci2s0Cfg.bMasterMode = FALSE;		// Slave mode
	 // Number of sub frames
	 stci2s0Cfg.bSubFrame01 = FALSE;		// 1 Sub frame with 2 channels
	 // FIFO configuration
	 stci2s0Cfg.bFifoTwoWords = TRUE;		// two 16 bit channels combined in one 32 bit word
	 //Base clock divider (master mode only)
	 stci2s0Cfg.bBclkDivByMclk = FALSE;
	 // bit extension
	 stci2s0Cfg.bBitExtensionHigh = FALSE;	// extend using zero (not relevant in our use case)
	 // output mode of the frame sync signal
	 stci2s0Cfg.bFreeRunMode = FALSE;		// burst mode (master only?)
	 // word bit shift order
	 stci2s0Cfg.bLsbFirst = FALSE;			//MSB first
	 // sampling point of data reception
	 stci2s0Cfg.bSampleAtEnd = FALSE;		// sample in the middle of data
	 // clock polarity
	 stci2s0Cfg.bClockpolarity = TRUE;		// drives data at falling edge and samples at rising edge of I2SCK
	 // frame sync phase
	 stci2s0Cfg.bWordSelectSamePhase = FALSE; // I2SWS is enabled one block before the first bit of frame data
	 // frame sync pulse width
	 stci2s0Cfg.bWordSelectLength = FALSE; 		// Pulse width will be one I2SCK cycle (1 bit); CODEC DSP mode
	 // frame sync polarity
	 stci2s0Cfg.bWordSelectPolarity = FALSE;	// I2SWS is 1, and the frame sync signal is enabled. This is 0 when idle.

	 // sub frame 0 configuration
	 stci2s0Cfg.stcSubframe0.u8Snchn = 1u;		// number of channels -1 for sub frame 0 (two channels)
	 stci2s0Cfg.stcSubframe0.u8Snchl = 15u;		// bit length -1 of the channels that make up sub frame 0 (32bit)
	 stci2s0Cfg.stcSubframe0.u8Snwdl = 15u;		// word length - 1, number of bits per word in the channel
	 // enable channels in sub frame 0
	 stci2s0Cfg.u32S0ch = 0x00000003u;			// enable ch 0 and ch 1 in sub frame 0

	 //FIFO thresholds
	 stci2s0Cfg.u8TxFifoThreshold = 4;			// FIFO tx threshold for generating an interrupt
	 stci2s0Cfg.u8RxFifoThreshold = 4;			// FIFO rx threshold for generating an interrupt

	 // packet receive completion timer
	 stci2s0Cfg.enPacketReceiveCompletionTimer = NoOperation;

	 // enable I2S Transmitter
	 stci2s0Cfg.bTxEnable = FALSE;				// will be enabled later
	 // enable I2S Receiver
	 stci2s0Cfg.bRxEnable = FALSE;				// will be enabled later

	 // Interrupt settings
	 stci2s0Cfg.bTxDmaEnable = TRUE;		// enable TX  DMA interrupt
	 stci2s0Cfg.bRxDmaEnable = TRUE;		// enable RX  DMA interrupt
	 bFM4_I2S0_DMAACT_RL1E0 = TRUE; 		// enable I2S receive DMA request
	 bFM4_I2S0_DMAACT_TL1E0 = TRUE;			// enable I2S transmit DMA request

	 stci2s0Cfg.pstcIrqEn = &stci2s0irqen;		// pointer to interrupt enable structure
	 stci2s0Cfg.pstcIrqCb = &stci2s0cb;			// pointer to interrupt callback structure = stc_i2s_intern_data_t
	 stci2s0Cfg.bTouchNvic = TRUE;				// enable access to NVIC


	 I2s_Init(&I2S0, &stci2s0Cfg);		// set all registers

}


/* 
 * Prepares the UART0 configuration structure provided by PDL library, enables UART0 pin function and
 * calls PDL drivers to initialize and start UART0
 * INPUT:
 *      baud_rate: Choose appropriate value, typical values for a pc system are 921600, 460800, 230400, 115200 etc.
 */
void init_uart0(uint32_t baud_rate)
{
	stc_mfs_uart_config_t stcMfsUartCfg; // Instantiate UART config structure
	
	// Clean UART config structure
	PDL_ZERO_STRUCT(stcMfsUartCfg);

	// populate UART config structure
		// UART Mode
	stcMfsUartCfg.enMode = UartNormal;
		// Baud rate
	stcMfsUartCfg.u32BaudRate = baud_rate;
		// Parity    : none
	stcMfsUartCfg.enParity = UartParityNone;
		// Stop bit  : 1bit
	stcMfsUartCfg.enStopBit = UartOneStopBit;
		// Data bits : 8bit
	stcMfsUartCfg.enDataLength = UartEightBits;
		// Bit direction : LSB first
	stcMfsUartCfg.enBitDirection = UartDataLsbFirst;
		// Inverted serial data format : NRZ
	stcMfsUartCfg.bInvertData = FALSE;
		// No HW flow control
	stcMfsUartCfg.bHwFlow = FALSE;
		// Internal clock
	stcMfsUartCfg.bUseExtClk = FALSE;
		// FIFO      : not use
	stcMfsUartCfg.pstcFifoConfig = NULL;

	    // Set pin function for UART0
    SetPinFunc_SIN0_0();
    SetPinFunc_SOT0_0();

		// Initialize UART 0
	if (Ok != Mfs_Uart_Init(&UART0, &stcMfsUartCfg))
	{
		while(1);
	}
		// Enable TX function of UART0
    Mfs_Uart_EnableFunc(&UART0, UartTx);
	    // Enable RX function of UART0
    Mfs_Uart_EnableFunc(&UART0, UartRx);

		// Write a test string
	//	writeUart0("Test Data");

}


/* 
 * Initializes the gpio ports for the 3 color LED, the user button and
 * the port for timing measurements (P10)
 */
void init_gpio(void)
{
	// init the ports the 3 color LED is connected to: Output with initial level high
	Gpio1pin_InitOut(GPIO1PIN_P1A, Gpio1pin_InitVal(HIGH));  	// Red color off
	Gpio1pin_InitOut(GPIO1PIN_PB2, Gpio1pin_InitVal(HIGH));		// Green color off
	Gpio1pin_InitOut(GPIO1PIN_P18, Gpio1pin_InitVal(HIGH));		// Blue color off

	// init test pin for time measurements with LA: Port P10, output, level high
	Gpio1pin_InitOut(GPIO1PIN_P10, Gpio1pin_InitVal(HIGH));

	// init port for user button: Port P20, input, no pull up
	Gpio1pin_InitIn(GPIO1PIN_P20, Gpio1pin_InitPullup(DISABLE));

}
