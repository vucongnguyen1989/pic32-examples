//  File:   keypad.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "config.h"
#include "keypad.h"

#define n_rows 4
#define n_cols 4

static bool matrix [n_rows][n_cols];

static uint translation [n_rows][n_cols] =
{
    {  1,   2,   3, 0xA,  },
    {  4,   5,   6, 0xB,  },
    {  7,   8,   9, 0xC,  },
    {  0, 0xF, 0xE, 0xD   }
};

#define buf_size 1024

static uchar buf [buf_size];
static int i_put, i_get;

static bool put (uchar a)
{
    int next_i_put = i_put + 1;

    if (next_i_put == buf_size)
        next_i_put = 0;

    if (next_i_put == i_get)
        return false;

    buf [i_put] = a;
    i_put = next_i_put;

    return true;
}

static void poll ()
{
    int row, col;

    for (col = 0; col < n_cols; col ++)
    {
        PORTEbits.RE3 = col != 0;
        PORTEbits.RE2 = col != 1;
        PORTEbits.RE1 = col != 2;
        PORTEbits.RE0 = col != 3;

        for (row = 0; row < n_rows; row ++)
        {
            bool on;

            switch (row)
            {
                case 0: on = ! PORTEbits.RE7; break;
                case 1: on = ! PORTEbits.RE6; break;
                case 2: on = ! PORTEbits.RE5; break;
                case 3: on = ! PORTEbits.RE4; break;
            }

            if (on && ! matrix [row][col])
                put (translation [row][col]);

            matrix [row][col] = on;
        }
    }
}

void keypad_init ()
{
    memset (matrix, 0, sizeof (matrix));
    i_put = i_get = 0;

    TRISE = 0xF0;
}

bool keypad_try_get (uchar * pa)
{
    if (i_get == i_put)
        return false;

    * pa = buf [i_get ++];

    if (i_get == buf_size)
        i_get = 0;

    return true;
}

uchar keypad_get ()
{
    uchar a;

    while (! keypad_try_get (& a))
        poll ();

    return a;
}
