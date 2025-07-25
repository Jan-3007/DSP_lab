/*
 * Author: Jan Eberhardt
 */

#include "global.h"



// using the hello_world_circ_buffer to verify whether the hardware setup is working correctly
CircularBuffer rx_buffer;
CircularBuffer tx_buffer;


// the following arrays/buffers are required in order to loop the data from the input to the output
uint32_t in[BLOCK_SIZE];
uint32_t out[BLOCK_SIZE];
int16_t left_in[BLOCK_SIZE];
int16_t right_in[BLOCK_SIZE];
int16_t left_out[BLOCK_SIZE];
int16_t right_out[BLOCK_SIZE];


int main()
{
    // initialze whole platform, does not start DMA
    init_platform(115200, hz32000, line_in);

    // use debug_printf() to send data to a Serial Monitor
    debug_printf("%s, %s\n", __DATE__, __TIME__);

    // function calls surrounded by IF_DEBUG() will be removed when building a Release
    IF_DEBUG(debug_printf("Hello World!\n"));

    // init test pin P10 to LOW; can be found on the board as part of the connector CN10, Pin is labelled as A3
    gpio_set(TEST_PIN, LOW);

    // initialize circular buffers
    rx_buffer.init();
    tx_buffer.init();

    memset(in, 0, sizeof(in));
    memset(out, 0, sizeof(out));


    // start I2S, call just before your main loop
    // this command starts the DMA, which will begin transferring data to and from the rx_buffer and tx_buffer
    platform_start();

    while(true)
    {
        // step 1: read block of samples from input buffer, data is copied from rx_buffer to in
        while(!rx_buffer.read(in));

        // blue LED is used to visualize (processing time)/(sample time)
        gpio_set(LED_B, LOW);			// LED_B on
        gpio_set(TEST_PIN, HIGH);       // Test Pin High

        // step 2: split samples into two channels
        convert_audio_sample_to_2ch(in, left_in, right_in);


        // step 3: process the audio channels
        //      3.1: convert from int to float if necessary, see CMSIS_DSP
        //      3.2: process data
        //      3.3: convert from float to int if necessary, see CMSIS_DSP
    
        // remove following for-loop when implementing your own audio processing
        for(uint32_t n = 0; n < BLOCK_SIZE; n++)
        {
            left_out[n] = left_in[n];
            right_out[n] = right_in[n];
        }
        

        // step 4: merge two channels into one sample
        convert_2ch_to_audio_sample(left_out, right_out, out);

        // step 5: write block of samples to output buffer, data is copied from out to tx_buffer
        while(!tx_buffer.write(out));

        gpio_set(LED_B, HIGH);			// LED_B off
        gpio_set(TEST_PIN, LOW);        // Test Pin Low
    }

    // fail-safe, never return from main on a microcontroller
    fatal_error();

    return 0;
}



// the following functions are called, when the DMA has finished transferring one block of samples and needs a new memory address to write/read to/from

// prototype defined in platform.h
// get new memory address to read new data to send it to DAC
uint32_t* get_new_tx_buffer_ptr()
{
    uint32_t* temp = tx_buffer.get_read_ptr();
    if(temp == nullptr)
    {
        fatal_error();
    }
    return temp;
}

// prototype defined in platform.h
// get new memory address to write new data received from ADC
uint32_t* get_new_rx_buffer_ptr()
{
    uint32_t* temp = rx_buffer.get_write_ptr();
    if(temp == nullptr)
    {
        fatal_error();
    }
    return temp;
}

