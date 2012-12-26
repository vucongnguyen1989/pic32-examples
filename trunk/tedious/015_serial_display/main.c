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

    display_init ();
    keypad_init (true);  // use_interrupts

    for (i = 0; i < 40; i++)
    {
        uchar c = keypad_get ();

        if (c == 0)
            display_new_line ();
        else
            display_hex_digit (keypad_get ());
    }
}

void main (void)
{
    running_fast ();

    for (;;)
        run ();
}
