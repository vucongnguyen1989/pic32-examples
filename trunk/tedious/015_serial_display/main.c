//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "config.h"
#include "delay.h"
#include "running.h"
#include "types.h"
#include "spi.h"
#include "uart.h"

void run (void)
{
    int i;

    spi_init (9600);  // baud
    keypad_init (false);  // use_interrupts

    spi_put_str ("\033[*");

    for (i = 0; i < 2000000; i++)
    {
        spi_put_str ("\033[0;0H");
        spi_put_hex_digit (keypad_get ());
    }
}

void main (void)
{
    running_fast ();

    for (;;)
        run ();
}
