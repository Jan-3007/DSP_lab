/*
 * utils.h
 *
 *  Created on: 07.02.2019
 *  original Author: Thomas Erforth
 * 
 * Modified and enhanced by: Jan Eberhardt
 */

#pragma once

// This enum describes some gpio resources on the FM4 pioneer platform
typedef enum
{
    LED_R,
    LED_G,
    LED_B,
    TEST_PIN,
    USER_BUTTON
} user_gpio;

void gpio_set(
    user_gpio gpio, 
    uint8_t level
    );

uint8_t gpio_get(user_gpio gpio);

void delay_ms(unsigned int ms);

void delay_us(unsigned int us);

void delay_cycles(unsigned int cycles);


// pseudo random sequence generator
#define NOISELEVEL 8000

short pseudo_random_sequence_generator(void);