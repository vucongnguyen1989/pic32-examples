//  File:   uart.c
//  Author: Yuri Panchul

#include <p32xxxx.h>
#include "uart.h"

void uart_init (uint hertz, uint baud)
{
    U1STAbits.UTXEN = 1;   // enable transmit pin
    U1STAbits.URXEN = 1;   // enable receive pin
    U1BRG           = hertz / 16 / baud - 1;
    U1MODEbits.ON   = 1;   // enable UART
}

void uart_putc (uchar c)
{
    while (U1STAbits.UTXBF);  // wait until transmit buffer empty
    U1TXREG = c;              // transmit character over UART
}

void uart_put_nl (void)
{
    uart_putc ('\r');
    uart_putc ('\n');
}

void uart_puts (uchar *s)
{
    while (*s != '\0')
        uart_putc (*s++);
}

void uart_putn (uint n)
{
    uint i;

    for (i = 1000 * 1000 * 1000; i >= 1; i /= 10)
    {
        if (n >= i || i == 1)
            uart_putc ('0' + n / i % 10);
    }
}
