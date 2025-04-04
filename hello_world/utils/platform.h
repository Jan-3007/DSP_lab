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

#include "hello_world_config.h"

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
    audio_input audio_in, 
    uint32_t tx_initial_buffer[BLOCK_SIZE], 
    uint32_t rx_initial_buffer[BLOCK_SIZE]
    );

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

void init_dstc(
    uint32_t tx_initial_buffer[BLOCK_SIZE], 
    uint32_t rx_initial_buffer[BLOCK_SIZE]
    );

void isr_tx();
void isr_rx();

// to be implemented by user
extern uint32_t* get_new_tx_buffer_ptr();
extern uint32_t* get_new_rx_buffer_ptr();



// Converts a uint32_t audio sample into two int16_t audio samples (left and right audio channel)
inline
void convert_audio_sample_to_2ch(
    uint32_t* audio_sample,         // input
    int16_t* left_sample,           // output
    int16_t* right_sample           // output
    )
{
    *left_sample = static_cast<int16_t>(*audio_sample >> 16);
    *right_sample = static_cast<int16_t>(*audio_sample);
}

