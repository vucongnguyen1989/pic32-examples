//  File:   main.c
//  Author: Yuri Panchul

#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "OledDriver.h"
#include "OledChar.h"
#include "OledGrph.h"

#include "display.h"

void main ()
{
    unsigned i;

#if 0
    OledInit       ();
/*
    OledMoveTo     (20, 10);
    OledDrawRect   (40, 15);
    OledPutString  ("abcdefghABCDEFGH");
    OledUpdate     ();
*/
    for (i = 0;; i++)
    {
        // OledMoveRight ();
        // OledMoveDown  ();
        // OledDrawRect  (40, 15);
        OledPutChar (i % 5 == 0 ? '\n' : 'a' + i % 26);
        OledUpdate    ();
    }



    display_init ();

    for (i = 0;; i++)
        display_char ('a' + i % 26);
#endif

    int x;
    double pi = atan (1) * 4;

    OledInit   ();
    OledMoveTo (0, 16);
/*
    OledMoveTo     (20, 10);
    OledDrawRect   (40, 15);
    OledPutString  ("abcdefghABCDEFGH");
    OledUpdate     ();
*/

    for (x = 0; x < 128; x++)
    {
        int y = 16 - (int) (sin (x * pi * 10 / 128) * 15);
        OledLineTo (x, y);
        OledMoveTo (x, y);
    }

    OledUpdate ();

    for (;;);
}
