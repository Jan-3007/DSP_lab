#include "global.h"



// using the hello_world_circ_buffer to verify whether the hardware setup is working correctly
CircularBuffer rx_buffer;
CircularBuffer tx_buffer;


int main()
{
    // initialze whole platform, starts DMA
    init_platform(115200, hz32000, line_in);

    // function calls surrounded by IF_DEBUG() will be removed when building a release
    IF_DEBUG(debug_printf("Hello World!\n"));
    debug_printf("%s, %s\n", __DATE__, __TIME__);

    // init test pin P10; CN10, labelled as A3
    gpio_set(TEST_PIN, LOW);

    // start I2S, call just before your main loop
    platform_start();

    int i = 0;
    while(true)
    {
        // the following arrays are necessary to be able to process the audio signal
        uint32_t input[BLOCK_SIZE];
        uint32_t output[BLOCK_SIZE];
        int16_t left_input[BLOCK_SIZE];
        int16_t right_input[BLOCK_SIZE];
        int16_t left_output[BLOCK_SIZE];
        int16_t right_output[BLOCK_SIZE];

        // step 1: read block of samples from input buffer
        while(!rx_buffer.read(input));

        // blue LED is used to visualize (processing time)/(sample time)
        gpio_set(LED_B, LOW);			// LED_B on
        gpio_set(TEST_PIN, HIGH);       // Test Pin High

        // step 2: split samples into two channels
        for(uint32_t i = BLOCK_SIZE; i > 0; i--)
        {
            convert_audio_sample_to_2ch(input, left_input, right_input);
        }


        // step 3: process the audio channels
        //      3.1: convert from int to float, see CMSIS_DSP
        //      3.2: process data
    
        // replace following for-loop with your audio processing
        for(uint32_t i = BLOCK_SIZE; i > 0; i--)
        {
            left_output[i] = left_input[i];
            right_output[i] = right_input[i];
        }
        //      3.3: convert from float to int, see CMSIS_DSP
        

        // step 4: merge two channels into one sample
        for(uint32_t i = BLOCK_SIZE; i > 0; i--)
        {
            convert_2ch_to_audio_sample(left_output, right_output, output);
        }

        // step 5: write block of samples to output buffer
        while(!tx_buffer.write(output));

        gpio_set(LED_B, HIGH);			// LED_B off
        gpio_set(TEST_PIN, LOW);        // Test Pin Low

        IF_DEBUG(debug_printf("i = %d\n", i));
        i++;
    }

    // fail-safe, never return from main on a microcontroller
    fatal_error();

    return 0;
}



// the following functions are called, when the DMA has finished transferring one block of samples and needs a new memory address to write/read to/from

// prototype defined in platform.h
// get new memory address to read from and send data to DAC
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
// get new memory address to write new data from ADC
uint32_t* get_new_rx_buffer_ptr()
{
    uint32_t* temp = rx_buffer.get_write_ptr();
    if(temp == nullptr)
    {
        fatal_error();
    }
    return temp;
}

