//  File:   main.c
//  Author: Yuri Panchul

#include <inttypes.h>
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
#endif

    display_init ();

    for (i = 0;; i++)
        display_char ('a' + i % 26);
}
