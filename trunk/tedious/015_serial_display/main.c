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

    TRISE = 0xf0;
    delay_seconds (1);

    spi_init (PBCLK_FREQUENCY, 115200);
    delay_seconds (1);

    spi_put_str ("\033[*");
    delay_seconds (1);

    for (i = 0; i < 20; i++)
    {
        spi_put_str ("\033[0;0H");
        spi_put_hex_digit (keypad_get ());
    }

    delay_seconds (1);
}

void main (void)
{
    running_fast ();

    for (;;)
        run ();
}
