//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

void delay (int n)
{
    n *= 10000;

    while (n --)
        asm ("nop");
}

void blink (void)
{
    int i;

    TRISE = 0;

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

void main (void)
{
    int config;

    for (;;)
    {
        // Fast

        CHECON    = 2;
        BMXCONCLR = 0x40;
        CHECONSET = 0x30;

        asm ("mfc0 %0, $16" : "=r" (config));
        config |= 3;
        asm ("mtc0 %0, $16" : "=r" (config));

        blink ();

        // Slow

        CHECON    = 7;
        BMXCONCLR = 0x40;
        CHECONCLR = 0x30;

        asm ("mfc0 %0, $16" : "=r" (config));
        config &= ~3;
        asm ("mtc0 %0, $16" : "=r" (config));

        blink ();
    }
}
