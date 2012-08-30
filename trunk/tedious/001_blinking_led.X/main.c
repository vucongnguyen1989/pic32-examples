/* 
 * File:   main.c
 * Author: panchul
 *
 * Created on August 30, 2012, 3:42 PM
 */

#include <p32xxxx.h>

void main (void)
{
    int i;

    TRISE = 0;

    for (;;)
    {
        for (i = 0; i < 10000; i++)
            asm ("nop");
        
        LATE = LATE + 1;
    }
}

