//  File:   main.c
//  Author: Yuri Panchul

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "OledDriver.h"
#include "OledChar.h"
#include "OledGrph.h"

void main ()
{
    unsigned i;

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
        OledPutChar ('a' + i % 26);
        OledUpdate    ();
    }
}
