/*
 * Author: Jan Eberhardt
 */

#pragma once


void write_uart0(const char* message);

// use for printing over UART0
void debug_printf(const char* fmt, ...);


// macro for disabling the given argument in Release build
#ifdef DEBUG
#define IF_DEBUG(x) x
#else
#define IF_DEBUG(x)
#endif



void fatal_error();

#ifdef DEBUG
#define ASSERT(cond)    if(!(cond)) fatal_error()
#else
#define ASSERT(cond)
#endif

