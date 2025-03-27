#include "global.h"
/*
#define BLOCK_SIZE            32
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
    // GPIO port configuration for 3 color LED, user button and test pin
	GpioInit();

    IF_DEBUG(Uart0Init(115200));

    IF_DEBUG(debug_printf("Hello World!\n"));

//    test_dsp_lib();

    int i = 0;
    while(true)
    {
        gpio_set(LED_B, LOW);			// LED_B on
        delay_ms(500);
        gpio_set(LED_B, HIGH);			// LED_B off
        delay_ms(500);

        IF_DEBUG(debug_printf("i = %d\n", i));
        i++;
    }

    return 0;
}

