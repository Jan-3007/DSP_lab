#include "global.h"
/*
#define NUM_TAPS_ARRAY_SIZE              32
#define NUM_TAPS 5

const float32_t firCoeffs32[NUM_TAPS_ARRAY_SIZE] = {
  -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
  -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
  +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
  +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f, 0.0f,0.0f,0.0f
};

static float32_t firStateF32[2 * BLOCK_SIZE + NUM_TAPS - 1];

void test_dsp_lib()
{
    arm_fir_instance_f32 fir_filter;
    arm_fir_init_f32(&fir_filter, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], BLOCK_SIZE);
}
*/




int main()
{
    init_buffer();

    // initialze whole platform, starts DMA, call as last init
    init_platform(115200, hz32000, line_in);


    // function calls surrounded by IF_DEBUG() will be removed when building a release
    IF_DEBUG(debug_printf("Hello World!\n"));

//    test_dsp_lib();

    int i = 0;
    while(true)
    {
        // using the hello_world_buffer to verify wether the hardware setup is working correctly
        // the following arrays are necessary with this buffer to be able to process the audio signal
        uint32_t input[BLOCK_SIZE];
        uint32_t output[BLOCK_SIZE];
        int16_t left_input[BLOCK_SIZE];
        int16_t right_input[BLOCK_SIZE];
        int16_t left_output[BLOCK_SIZE];
        int16_t right_output[BLOCK_SIZE];

        // step 1: read block of samples from input buffer
        while(rx_buffer_read(input));

        // blue LED is used to visualize (processing time)/(sample time)
        gpio_set(LED_B, LOW);			// LED_B on

        // step 2: split samples into two channels
        for(uint32_t i = BLOCK_SIZE; i > 0; i--)
        {
            convert_audio_sample_to_2ch(input, left_input, right_input);
        }

        // step 3: process the audio channels
        //      3.1: convert from int to float, see CMSIS DSP
        //      3.2: process data
        //
        // replace following for-loop with your audio processing
        for(uint32_t i = BLOCK_SIZE; i > 0; i--)
        {
            left_output[i] = left_input[i];
            right_output[i] = right_input[i];
        }
        //      3.3: convert from float to int, see CMSIS DSP
        
        // step 4: merge two samples into one
        for(uint32_t i = BLOCK_SIZE; i > 0; i--)
        {
            convert_2ch_to_audio_sample(left_output, right_output, output);
        }

        // step 5: write block of samples to output buffer
        while(tx_buffer_write(output));

        gpio_set(LED_B, HIGH);			// LED_B off

        // print to test UART
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
    return tx_buffer_get_read_ptr();
}

// prototype defined in platform.h
// get new memory address to write new data from ADC
uint32_t* get_new_rx_buffer_ptr()
{
    return rx_buffer_get_write_ptr();
}

