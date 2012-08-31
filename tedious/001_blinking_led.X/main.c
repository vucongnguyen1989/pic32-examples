//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

void delay (int n)
{
    n *= 10000;

    while (n --)
        asm ("nop");
}

void main (void)
{
    int i;

    TRISE = 0;

    for (;;)
    {
        // Output uses LAT register

        LATE = 0;

        for (i = 0; i < 16; i++)
        {
            delay (1);
            LATE ++;
        }

        LATE = 0;

        for (i = 0; i < 256; i += 16)
        {
            delay (1);
            LATE = i;
        }

        LATE = 0;

        // Output uses PORT register

        PORTE = 1;

        for (i = 0; i < 8; i++)
        {
            delay (1);
            PORTE <<= 1;
        }

        PORTE = 0;

        // Output uses PORTxSET, PORTxCLR and PORTxINV registers

        for (i = 0; i < 4; i++)
        {
            delay (1);
            PORTESET = (1 << i) | (0x80 >> i);
        }

        for (i = 0; i < 4; i++)
        {
            delay (1);
            PORTECLR = (1 << i) | (0x80 >> i);
        }

        delay (1);
        PORTESET = 0xAA;

        for (i = 0; i < 8; i++)
        {
            delay (1);
            PORTEINV = 1 << i;
        }
    }
}

