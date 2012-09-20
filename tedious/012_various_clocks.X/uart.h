//  File:   uart.h
//  Author: Yuri Panchul

#ifndef UART_H
#define UART_H

#include "types.h"

void uart_init   (uint hertz, uint baud);
void uart_putc   (uchar c);
void uart_put_nl (void);
void uart_puts   (uchar *s);
void uart_putn   (uint n);

#endif
