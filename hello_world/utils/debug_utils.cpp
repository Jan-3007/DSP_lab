#include "global.h"
#include <cstdarg>


// write_uart0: transmits a string to UART0 transmit buffer
void write_uart0(const char* message)
{
	while(*message != 0)
	{
	    while (TRUE != Mfs_Uart_GetStatus(&UART0, UartTxEmpty)); //wait until TX buffer empty
	    Mfs_Uart_SendData(&UART0, *message);

	    message++;
	}
}


void debug_printf(const char* fmt, ...)
{
    std::va_list args;
    va_start(args, fmt);

    char buffer[80];
    vsnprintf(buffer, sizeof(buffer), fmt, args);  

    write_uart0(buffer);

    va_end(args);
}


