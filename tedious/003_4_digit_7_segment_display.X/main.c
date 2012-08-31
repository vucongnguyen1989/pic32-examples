//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

void delay (int n)
{
    n *= 10000;

    while (n --)
        asm ("nop");
}

/*
 * port  display  segment
 *       pin
 *
 * D1    1        E
 * F1    2        D
 * D0    3        DP
 * D8    4        C
 * F3    5        G
 * F2    6        drain 4
 *
 * F4    7        B
 * F6    8        drain 3
 * D7    9        drain 2
 * D6    10       F
 * D11   11       A
 * D5    12       drain 1
 *
 */

void setup_display_abcdef_dp ()
{
    TRISDCLR = (  1 << 11) | (  1 << 8) | (  1 << 6) | (  1 << 1) |   1;
    TRISFCLR = (  1 <<  4) | (  1 << 3) | (  1 << 1);
}

void display_abcdef_dp (int a, int b, int c, int d, int e, int f, int g, int dp)
{
    PORTDCLR = (! a << 11) | (! c << 8) | (! f << 6) | (! e << 1) | ! dp;
    PORTFCLR = (! b <<  4) | (! g << 3) | (! d << 1);

    PORTDSET = (  a << 11) | (  c << 8) | (  f << 6) | (  e << 1) |   dp;
    PORTFSET = (  b <<  4) | (  g << 3) | (  d << 1);

    PORTE = (a << 7) | (b << 6) | (c << 5) | (d << 4) | (e << 3) | (f << 2) | (g << 1) | dp;
}

void setup_set_digit ()
{
    TRISDCLR = (1 << 5) | (1 << 7);
    TRISFCLR = (1 << 6) | (1 << 2);
}

void set_digit (int digit)
{
    switch (digit)
    {
        case 1:

            PORTDCLR = 1 << 5;
            PORTDSET = 1 << 7;
            PORTFSET = (1 << 6) | (1 << 2);

            break;

        case 2:

            PORTDCLR = 1 << 7;
            PORTDSET = 1 << 5;
            PORTFSET = (1 << 6) | (1 << 2);

            break;

        case 3:

            PORTFCLR = 1 << 6;
            PORTFSET = 1 << 2;
            PORTDSET = (1 << 5) | (1 << 7);

            break;

        case 4:

            PORTFCLR = 1 << 2;
            PORTFSET = 1 << 6;
            PORTDSET = (1 << 5) | (1 << 7);

            break;
    }
}

void display_digit (int n)
{
    switch (n)
    {
        case 0x0: display_abcdef_dp (1, 1, 1, 1, 1, 1, 0, 0); break;
        case 0x1: display_abcdef_dp (0, 1, 1, 0, 0, 0, 0, 0); break;
        case 0x2: display_abcdef_dp (1, 1, 0, 1, 1, 0, 1, 0); break;
        case 0x3: display_abcdef_dp (1, 1, 1, 1, 0, 0, 1, 0); break;
        case 0x4: display_abcdef_dp (0, 1, 1, 0, 0, 1, 1, 0); break;
        case 0x5: display_abcdef_dp (1, 0, 1, 1, 0, 1, 1, 0); break;
        case 0x6: display_abcdef_dp (1, 0, 1, 1, 1, 1, 1, 0); break;
        case 0x7: display_abcdef_dp (1, 1, 1, 0, 0, 0, 0, 0); break;
        case 0x8: display_abcdef_dp (1, 1, 1, 1, 1, 1, 1, 0); break;
        case 0x9: display_abcdef_dp (1, 1, 1, 1, 0, 1, 1, 0); break;
        case 0xA: display_abcdef_dp (1, 1, 1, 0, 1, 1, 1, 0); break;
        case 0xB: display_abcdef_dp (0, 0, 1, 1, 1, 1, 1, 0); break;
        case 0xC: display_abcdef_dp (1, 0, 0, 1, 1, 1, 0, 0); break;
        case 0xD: display_abcdef_dp (0, 1, 1, 1, 1, 0, 1, 0); break;
        case 0xE: display_abcdef_dp (1, 0, 0, 1, 1, 1, 1, 0); break;
        case 0xF: display_abcdef_dp (1, 0, 0, 0, 1, 1, 1, 0); break;
    }
}

void main (void)
{
    int i, j;

    setup_display_abcdef_dp ();
    setup_set_digit         ();

    for (;;)
    for (i = 0;; i = (i + 1) % 4)
    {
        set_digit (i + 1);

        for (j = 0; j < 16; j++)
        {
            display_digit (j);
            delay (10);
        }
    }
}
