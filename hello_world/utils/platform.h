/* platform.h
 * Platform initialization, enables access to platform resources like I2S0, audio CODEC, UART0 and gpio
 *
 *  Copyright (C) 2019 Thomas Erforth
 *
 * Change history:
 * ter, Jan 30, 2019: Initial version
 * ter, Apr 09, 2019: enum mode: poll added
 * ter, July 21, 2019: Declarations for dma_buffers, DSTC and slider app moved to platform.c and declared extern here
 * 					   Define directive for PING / PONG added
 *
 */
#pragma once

/*! This enum describes the different sampling frequency setup of the CODEC */
typedef enum
{
    hz8000 	= 0x0C,  // 8kHz from 12.288MHz MCLK
    hz32000 = 0x18,  // 32kHz from 12.288MHz MCLK
	hz48000 = 0x00,  // 48kHz from 12.288MHz MCLK
    hz96000 = 0x1C,  // 96kHz from 12.288MHz MCLK
} sampling_rate;

/*! This enum describes the different input audio jacks of the CODEC */		
typedef enum
{
	line_in = 0x12,  // Select Line In and disable bypass
    mic_in 	= 0x15,  // Select Microphone In and disable bypass
} audio_input;


void init_platform (
    uint32_t baud_rate, 
    sampling_rate sample_rate, 
    audio_input audio_in
    );

void platform_start();

void init_uart0(
    uint32_t baud_rate
    );

void init_codec(
    sampling_rate codec_sample_rate, 
    audio_input audio_in
    );

void init_gpio(void);

void init_I2S0(
    sampling_rate sample_rate
    );

void init_dstc();

void isr_tx();
void isr_rx();

// to be implemented by user
extern uint32_t* get_new_tx_buffer_ptr();
extern uint32_t* get_new_rx_buffer_ptr();



// Converts a uint32_t audio sample into two int16_t audio samples (left and right audio channel)
// for arrays of length BLOCK_SIZE
inline
void convert_audio_sample_to_2ch(
    uint32_t input[BLOCK_SIZE],                 // input
    int16_t left_channel[BLOCK_SIZE],           // output
    int16_t right_channel[BLOCK_SIZE]           // output
    )
{
    for(uint32_t n = BLOCK_SIZE; n > 0; n--)
    {
        *left_channel++ = static_cast<int16_t>( static_cast<uint16_t>(*input >> 16) );
        *right_channel++ = static_cast<int16_t>( static_cast<uint16_t>(*input) );
        input++;
    }
}

// Converts two int16_t audio samples (left and right audio channel) into a uint32_t audio sample
// for arrays of length BLOCK_SIZE
inline
void convert_2ch_to_audio_sample(
    int16_t left_channel[BLOCK_SIZE],           // input
    int16_t right_channel[BLOCK_SIZE],          // input
    uint32_t output[BLOCK_SIZE]                 // output
    )
{
    for(uint32_t n = BLOCK_SIZE; n > 0; n--)
    {
        uint32_t left_temp = static_cast<uint32_t>( static_cast<uint16_t>(*left_channel) );
        uint32_t right_temp = static_cast<uint32_t>( static_cast<uint16_t>(*right_channel) );
        *output++ = (left_temp << 16) | right_temp;
        left_channel++;
        right_channel++;
    }
}