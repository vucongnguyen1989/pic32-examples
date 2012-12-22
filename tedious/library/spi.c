//  File:   spi.c
//  Author: Yuri Panchul
//
//  This code uses fragments adopted from
//  Digital Design and Computer Architecture, Second Edition
//  by David Harris & Sarah Harris

#include <p32xxxx.h>

#include "config.h"
#include "spi.h"

void spi_init (void)
{
    char dummy;

    SPI1CONbits.ON     = 0;        // disable SPI to reset any previous state
    dummy              = SPI2BUF;  // clear receive buffer
    SPI1BRG            = 7;
    SPI1CONbits.MSTEN  = 1;        // enable master mode
    SPI1CONbits.CKE    = 1;        // set clock-to-data timing
    SPI1CONbits.ON     = 1;        // turn SPI on
}

uchar spi_put_get_char (uchar c)
{
    SPI1BUF = c;                   // send data to slave
    while (SPI1STATbits.SPIBUSY);  // wait until SPI transmission complete
    return SPI1BUF;
}

uchar spi_get_char (void)
{
    return spi_put_get (0);
}

void spi_put_char (uchar c)
{
    (void) spi_put_get (c);
}

void spi_put_new_line (void)
{
    spi_put_char ('\r');
    spi_put_char ('\n');
}

void spi_put_str (uchar *s)
{
    while (*s != '\0')
        spi_put_char (*s++);
}

void spi_put_dec (uint n)
{
    uint i;

    for (i = 1000 * 1000 * 1000; i >= 1; i /= 10)
    {
        if (n >= i || i == 1)
            spi_put_char ('0' + n / i % 10);
    }
}

void spi_put_hex_digit (uint n)
{
    uchar c;

    c  = n & 0x0f;
    c += c >= 10 ? 'A' - 10 : '0';

    spi_put_char (c);
}

void spi_put_hex (uint n)
{
    uint i;

    for (i = 0; i < sizeof (n) * 2; i++)
        spi_put_hex_digit (n >> (sizeof (n) * 2 - 1 - i) * 4);
}
