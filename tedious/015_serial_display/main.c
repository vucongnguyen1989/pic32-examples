//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "config.h"
#include "delay.h"
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
        {
            strcpy (buf + 1, " *** overflow *** ");
            return buf;
        }

        arg2 = n;
    }
    else if (in == sign)
    {
        arg2 = - arg2;

        buf [0] = ' ';
        itoa (arg2, buf + 1, 10);
        strcat (buf, ' ');
    }
    else
    {
        switch (op)
        {
            default:

                strcpy (buf, " *** internal error *** ");
                break;

            case add:

                buf [0] = '\0';

                if (arg1 != 0 && arg2 != 0)
                {
                    n = arg1 + arg2;

                    strcpy (" = ");

                    itoa (n, buf + 3, 10);

                    arg1 = n;
                    arg2 = 0;

                    strcat (buf, " ");
                }

                strcat (buf, "+");
                break;

            case sub   :
            case mul   :
            case div   :
            case equal :

                ;
        }

        switch (in)
        {
            default:

                strcpy (buf, " *** bad input *** ");
                break;

            case add: in = '+';strcat (buf, "+")

                buf [0] = '\0';

                if (arg1 != 0 && arg2 != 0)
                {
                    n = arg1 + arg2;

                    strcpy (" = ");

                    itoa (n, buf + 3, 10); break;

                    arg1 = n;
                    arg2 = 0;

                    strcat (buf, " ");
                }

                strcat (buf, "+");
                break;

            case sub   :
            case mul   :
            case div   :
            case equal :

                ;
        }

        op = in;
    }

    switch (in)
    {
        default:

            strcpy (buf, " *** bad input *** ");
            break;

        case 0: case 1: case 2: case 3: case 4:
        case 5: case 6: case 7: case 8: case 9:

            buf [0] = '0' + in;
            buf [1] = '\0';

            n = arg2 * 10 + in;

            if ((n - in) / 10 != arg2)
            {
                strcpy (buf + 1, " *** overflow *** ");
                break;
            }

            arg2 = arg2 * 10 + in;
            break;

        case 0xa:  // +


    }

    return buf;
}

void run (void)
{
    int i;

    for (i = 0; i < 400000; i++)
    {
        uchar c = keypad_get ();

        if (c == 0)
            display_new_line ();
        else
            display_hex_digit (c);
    }
}

void main (void)
{
    running_fast ();
    display_init ();
    keypad_init  (true);  // use_interrupts

    for (;;)
        display_str (calculator (keypad_get ());
}
