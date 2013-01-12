//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "config.h"
#include "delay.h"
#include "memory.h"
#include "running.h"
#include "types.h"
#include "spi.h"
#include "uart.h"

#define add    0xa
#define sub    0xb
#define mul    0xc
#define div    0xd
#define sign   0xe
#define equal  0xf

static void calculator_itoa (int n, uchar * buf)
{
    uint i;

    if (n < 0)
    {
        * buf ++ = '-';
        n = - n;
    }

    for (i = 1000 * 1000 * 1000; i >= 1; i /= 10)
    {
        if (n >= i || i == 1)
            * buf ++ = '0' + n / i % 10;
    }

    * buf = '\0';
}

uchar * calculator (uchar in)
{
    static uchar buf [32];

    static int   arg1 = 0;
    static int   arg2 = 0;
    static int   op   = add;

    int n;

    if (in <= 9)
    {
        buf [0] = '0' + in;
        buf [1] = '\0';

        n = arg2 * 10 + in;

        if ((n - in) / 10 != arg2)
            goto OVERFLOW;

        arg2 = n;
    }
    else if (in == sign)
    {
        arg2 = - arg2;

        buf [0] = '=';
        calculator_itoa (arg2, buf + 1);
    }
    else
    {
        switch (op)
        {
            default:

                goto INTERNAL_ERROR;

            case add:

                n = arg1 + arg2;

                if (    arg1 > 0 && arg2 > 0 && n < 0
                     || arg1 < 0 && arg2 < 0 && n > 0)
                {
                    goto OVERFLOW;
                }

                break;

            case sub:

                arg2 = - arg2;

                n = arg1 + arg2;

                if (    arg1 > 0 && arg2 > 0 && n < 0
                     || arg1 < 0 && arg2 < 0 && n > 0)
                {
                    goto OVERFLOW;
                }

                break;

            case mul:

                n = arg1 * arg2;

                if (arg2 != 0 && n / arg2 != arg1)
                    goto OVERFLOW;

                break;

            case div:

                if (arg2 == 0)
                    goto OVERFLOW;

                n = arg1 / arg2;
                break;

            case equal:

                n = 0;
                break;
        }

        buf [0] = '=';

        calculator_itoa (n, buf + 1);

        arg1 = n;
        arg2 = 0;

        switch (in)
        {
            default:     goto INTERNAL_ERROR;
            case add:    strcat (buf, "+"); break;
            case sub:    strcat (buf, "-"); break;
            case mul:    strcat (buf, "*"); break;
            case div:    strcat (buf, "/"); break;
            case equal:  break;
        }

        op = in;
    }

    return buf;

    OVERFLOW:

    strcat (buf, "[overflow]");
    goto RESET;

    INTERNAL_ERROR:

    strcat (buf, "[internal error]");
    goto RESET;

    RESET:

    arg1 = 0;
    arg2 = 0;
    op   = add;

    return buf;
}

void main (void)
{
    int i;

    running_fast ();
    display_init ();
    keypad_init  (true);  // use_interrupts

    for (;;)
        display_str (calculator (keypad_get ()));
}
