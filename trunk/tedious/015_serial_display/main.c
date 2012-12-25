//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "config.h"
#include "delay.h"
#include "running.h"
#include "types.h"
#include "spi.h"

void f (void)
{
    int i;

    for (i = ' '; i <= 255; i++)
    {
        if ((i - ' ') % 32 == 0)
            spi_put_str ("\033[0;0H");

        if ((i - ' ') % 32 == 16)
            spi_put_str ("\033[1;0H");

        spi_put_char ((uchar) i);
        delay_millis (200);
    }

    delay_seconds (10);
}

void g (void)
{
    int i;

//    spi_put_str ("\033[0;0Habcdefghijklmnop");
    spi_put_str ("\033[0;0H1111156789222225678933333567894444456789");
    spi_put_str ("\033[1;0H0123456789abcdef");
    delay_millis (1000);

    for (i = 1; i < 50; i++)
    {
        spi_put_str ("\033[");
        spi_put_dec (1);
        spi_put_str ("@");

        delay_millis (1000);
    }

    for (i = 1; i < 60; i++)
    {
        spi_put_str ("\033[");
        spi_put_dec (1);
        spi_put_str ("A");

        delay_millis (1000);
    }

    spi_put_str ("\033[0;0Hxxxxxxxxxxxxxxxx");
    spi_put_str ("\033[1;0Hyyyyyyyyyyyyyyyy");
    delay_millis (1000);
}

void run (void)
{
    int i;

    delay_seconds (1);
    spi_init ();

    delay_seconds (1);
  //  spi_put_str ("\033[*");
    delay_seconds (1);

    spi_put_str ("abcdefghijklmnop");
    delay_seconds (1);
    /*
  spi_put_str ("\033[0e");
    spi_put_str ("\033[1;0H0123456789abcdef");
    delay_seconds (1);
  spi_put_str ("\033[1e");
    spi_put_str ("\033[1;0H0123456789abcdef");
    delay_seconds (1);
  spi_put_str ("\033[2e");
    spi_put_str ("\033[1;0H0123456789abcdef");
    delay_seconds (1);
  spi_put_str ("\033[3e");
    spi_put_str ("\033[1;0H0123456789abcdef");
    delay_seconds (1);
*/
//    spi_put_str ("\033[0h");
//    delay_seconds (1);
//    g ();

    //delay_seconds (10);
}

void main (void)
{
    running_fast ();

    for (;;)
        run ();
}
