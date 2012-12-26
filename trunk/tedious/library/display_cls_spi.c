//  File:   display_cls_spi.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "config.h"
#include "spi.h"
#include "display.h"

static uchar buf [16];
static int col;

void display_init (void)
{
    spi_init (9600);  // baud

    memset (buf, ' ', sizeof (buf));

    col = 0;

//    spi_put_str ("\033[*");     // reset
//    spi_put_str ("\033[0h");    // set display mode - wrap line at 16 characters
//    spi_put_str ("\033[2c");    // set cursor mode - cursor on, blink on
    spi_put_str ("\033[1;0H");  // set cursor position to row 1 column 0
}

static void scroll ()
{
    int i;

    spi_put_str ("\033[0;0H");  // set cursor position to row 0 column 0

    for (i = 0; i < sizeof (buf); i++)
        spi_put_char (buf [i]);

    spi_put_str ("\033[1;0H");  // set cursor position to row 1 column 0

    for (i = 0; i < sizeof (buf); i++)
        spi_put_char (' ');

    spi_put_str ("\033[1;0H");  // set cursor position to row 1 column 0
    col = 0;
}

void display_char (uchar c)
{
    if (c == '\n')
    {
        scroll ();
        return;
    }

    spi_put_char (c);
    buf [col ++] = c;

    if (col == sizeof (buf))
        scroll ();
}

void display_new_line (void)
{
    display_char ('\n');
}

void display_str (uchar *s)
{
    while (*s != '\0')
        display_char (*s++);
}

void display_dec (uint n)
{
    uint i;

    for (i = 1000 * 1000 * 1000; i >= 1; i /= 10)
    {
        if (n >= i || i == 1)
            display_char ('0' + n / i % 10);
    }
}

void display_hex_digit (uint n)
{
    uchar c;

    c  = n & 0x0f;
    c += c >= 10 ? 'A' - 10 : '0';

    display_char (c);
}

void display_hex_byte (uchar n)
{
    uint i;

    for (i = 0; i < sizeof (n) * 2; i++)
        display_hex_digit (n >> (sizeof (n) * 2 - 1 - i) * 4);
}

void display_hex (uint n)
{
    uint i;

    for (i = 0; i < sizeof (n) * 2; i++)
        display_hex_digit (n >> (sizeof (n) * 2 - 1 - i) * 4);
}
