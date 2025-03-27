/*
 * utils.h
 *
 *  Created on: 07.02.2019
 *      Author: Thomas Erforth
 */

#ifndef SYSTEM_INCLUDE_PLATFORM_UTILS_H_
#define SYSTEM_INCLUDE_PLATFORM_UTILS_H_

#include <pdl_header.h>

#define CLK_FREQ 200000000UL

// This enum describes some gpio resources on the FM4 pioneer platform
 typedef enum
 {
	 LED_R,
	 LED_G,
	 LED_B,
	 TEST_PIN,
	 USER_BUTTON
 } userGPIO;

// prototypes
void gpio_set(userGPIO, uint8_t);
uint8_t gpio_get(userGPIO gpio);
void writeUart0(uint8_t message[]);
short prbs(void);
void delay_ms(unsigned int ms);
void delay_us(unsigned int us);
void delay_cycles(unsigned int cycles);

#endif /* SYSTEM_INCLUDE_PLATFORM_UTILS_H_ */
