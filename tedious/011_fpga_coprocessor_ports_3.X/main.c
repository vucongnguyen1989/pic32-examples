//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "config.h"
#include "delay.h"
#include "running.h"
#include "types.h"
#include "uart.h"

#define PORTD_NO_RESET      0x20
#define PORTD_TAG           0x40

void fpga_init (void)
{
    int i;

    AD1PCFG = ~0;  // No analog ports

    TRISE = 0x00;  // PORT E is an output
    TRISD = 0x1F;  // PORT D [7:5] - output, [3:0] - input

    delay_millis (1);

    PORTD = ~ PORTD_NO_RESET;

    delay_millis (1);

    PORTD = PORTD_NO_RESET;

    delay_millis (1);
}

uint calculate_expected_result (uint n)
{
    n = (n * n) + 3;
    return (n * n) & 0xF;
}

void output_result
(
    uint number_of_nops,
    uint argument,
    uint result    
)
{
    uint expected_result = calculate_expected_result (argument);

    if (result == expected_result)
        return;

    uart_put_dec      (number_of_nops);
    uart_put_str      (" nops ");
    uart_put_dec      (argument);
    uart_put_char     (' ');
    uart_put_dec      (result);
    uart_put_char     (' ');
    uart_put_dec      (expected_result);
    uart_put_new_line ();
}

void run (void)
{
    uint n, r;

    uart_init (PBCLK_FREQUENCY, 9600);

    fpga_init ();

    for (n = 0; n < 256; n++)
    {
        PORTE = n;
        PORTD = PORTD_NO_RESET | (n & 1 ? 0 : PORTD_TAG);
        asm volatile ("nop; nop; nop; nop");
        r = PORTD & 0xF;
        output_result (4, n, r);
    }

    fpga_init ();

    for (n = 0; n < 256; n++)
    {
        PORTE = n;
        PORTD = PORTD_NO_RESET | (n & 1 ? 0 : PORTD_TAG);
        asm volatile ("nop; nop; nop; nop; nop");
        r = PORTD & 0xF;
        output_result (5, n, r);
    }
}

void main (void)
{
    uint i;

    uart_init (PBCLK_FREQUENCY, 9600);
    running_fast ();

    for (i = 0;; i++)
    {
        delay_seconds     (1);
        uart_put_dec      (i);
        uart_put_new_line ();
    }

    for (;;)
        run ();
}
