//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "config.h"
#include "delay.h"
#include "running.h"
#include "types.h"
#include "spi.h"

void run (void)
{
    spi_init ();
    spi_put_str ("Hello, world");
    delay_seconds (1);
}

void main (void)
{
    running_fast ();

    for (;;)
        run ();
}
